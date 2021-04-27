#include "include/platform.h"
#include "include/timer.h"
#include "include/sysctl.h"
#include "include/encoding.h"
#include "include/riscv.h"
#include "include/clint.h"
#include "include/printk.h"
#include "include/plic.h"
#include "include/assert.h"
#include <stdint.h>
#include <stddef.h>
typedef int bool;

typedef struct timer_instance
{
    timer_callback_t callback;
    void *ctx;
    bool single_shot;
} timer_instance_t;
volatile timer_instance_t timer_instance[TIMER_DEVICE_MAX][TIMER_CHANNEL_MAX];

volatile kendryte_timer_t *const timer[3] =
    {
        (volatile kendryte_timer_t *)TIMER0_BASE_ADDR,
        (volatile kendryte_timer_t *)TIMER1_BASE_ADDR,
        (volatile kendryte_timer_t *)TIMER2_BASE_ADDR
    };

void timer_init(timer_device_number_t timer_number) {
    for(size_t i = 0; i < TIMER_CHANNEL_MAX; i++)
        timer_instance[timer_number][i] = (const timer_instance_t){
            .callback = NULL,
            .ctx = NULL,
            .single_shot = 0,
        };
    sysctl_clock_enable(SYSCTL_CLOCK_TIMER0 + timer_number);
}

size_t timer_set_interval(timer_device_number_t timer_number, timer_channel_number_t channel, size_t nanoseconds)
{
    uint32_t clk_freq = sysctl_clock_get_freq(SYSCTL_CLOCK_TIMER0 + timer_number);
    size_t min_step = 1000000000 / clk_freq;
    size_t value = nanoseconds / min_step;
    assert(value > 0 && value < UINT32_MAX);
    timer[timer_number]->channel[channel].load_count = (uint32_t)value;
    return (min_step * value);
}
#define readl(addr) (*(volatile uint32_t *)(addr))
typedef void (*timer_ontick)();
timer_ontick time_irq[3][4] = {NULL};
static int timer_isr(void *parm)
{
    uint32_t timer_number;
    for(timer_number = 0; timer_number < 3; timer_number++)
    {
        if(parm == timer[timer_number])
            break;
    }

    uint32_t channel = timer[timer_number]->intr_stat;
    size_t i = 0;
    for(i = 0; i < 4; i++)
    {
        if(channel & 1)
        {
            if(time_irq[timer_number][i])
                (time_irq[timer_number][i])();
            break;
        }

        channel >>= 1;
    }

    readl(&timer[timer_number]->eoi);
    return 0;
}

void timer_set_irq(timer_device_number_t timer_number, timer_channel_number_t channel, void (*func)(), uint32_t priority)
{
    time_irq[timer_number][channel] = func;
    if(channel < 2)
    {
        plic_set_priority(IRQN_TIMER0A_INTERRUPT + timer_number * 2, priority);
        plic_irq_register(IRQN_TIMER0A_INTERRUPT + timer_number * 2, timer_isr, (void *)timer[timer_number]);
        plic_irq_enable(IRQN_TIMER0A_INTERRUPT + timer_number * 2);
    } else
    {
        plic_set_priority(IRQN_TIMER0B_INTERRUPT + timer_number * 2, priority);
        plic_irq_register(IRQN_TIMER0B_INTERRUPT + timer_number * 2, timer_isr, (void *)timer[timer_number]);
        plic_irq_enable(IRQN_TIMER0B_INTERRUPT + timer_number * 2);
    }
}

void timer_set_enable(timer_device_number_t timer_number, timer_channel_number_t channel, uint32_t enable)
{
    if(enable)
        timer[timer_number]->channel[channel].control = TIMER_CR_USER_MODE | TIMER_CR_ENABLE;
    else
        timer[timer_number]->channel[channel].control = TIMER_CR_INTERRUPT_MASK;
}