#ifndef __TIMER_H
#define __TIMER_H
#include <stdint.h>
#include <stddef.h>

//ref standalone timer.h

/* clang-format off */
typedef struct _timer_channel
{
    /* TIMER_N Load Count Register              (0x00+(N-1)*0x14) */
    volatile uint32_t load_count;
    /* TIMER_N Current Value Register           (0x04+(N-1)*0x14) */
    volatile uint32_t current_value;
    /* TIMER_N Control Register                 (0x08+(N-1)*0x14) */
    volatile uint32_t control;
    /* TIMER_N Interrupt Clear Register         (0x0c+(N-1)*0x14) */
    volatile uint32_t eoi;
    /* TIMER_N Interrupt Status Register        (0x10+(N-1)*0x14) */
    volatile uint32_t intr_stat;
} __attribute__((packed, aligned(4))) timer_channel_t;

typedef struct _kendryte_timer
{
    /* TIMER_N Register                         (0x00-0x4c) */
    volatile timer_channel_t channel[4];
    /* reserverd                                (0x50-0x9c) */
    volatile uint32_t resv1[20];
    /* TIMER Interrupt Status Register          (0xa0) */
    volatile uint32_t intr_stat;
    /* TIMER Interrupt Clear Register           (0xa4) */
    volatile uint32_t eoi;
    /* TIMER Raw Interrupt Status Register      (0xa8) */
    volatile uint32_t raw_intr_stat;
    /* TIMER Component Version Register         (0xac) */
    volatile uint32_t comp_version;
    /* TIMER_N Load Count2 Register             (0xb0-0xbc) */
    volatile uint32_t load_count2[4];
} __attribute__((packed, aligned(4))) kendryte_timer_t;

typedef enum _timer_deivce_number
{
    TIMER_DEVICE_0,
    TIMER_DEVICE_1,
    TIMER_DEVICE_2,
    TIMER_DEVICE_MAX,
} timer_device_number_t;

typedef enum _timer_channel_number
{
    TIMER_CHANNEL_0,
    TIMER_CHANNEL_1,
    TIMER_CHANNEL_2,
    TIMER_CHANNEL_3,
    TIMER_CHANNEL_MAX,
} timer_channel_number_t;

/* TIMER Control Register */
#define TIMER_CR_ENABLE             0x00000001
#define TIMER_CR_MODE_MASK          0x00000002
#define TIMER_CR_FREE_MODE          0x00000000
#define TIMER_CR_USER_MODE          0x00000002
#define TIMER_CR_INTERRUPT_MASK     0x00000004
#define TIMER_CR_PWM_ENABLE         0x00000008
/* clang-format on */

extern volatile kendryte_timer_t *const timer[3];

/**
 * @brief       Definitions for the timer callbacks
 */
typedef int (*timer_callback_t)(void *ctx);

/**
 * @brief       Init timer
 *
 * @param[in]   timer       timer
 */
void timer_init(timer_device_number_t timer_number);

/**
 * @brief       [DEPRECATED] Set timer timeout function
 *
 * @param[in]   timer           timer
 * @param[in]   channel         channel
 * @param[in]   func            timeout function
 * @param[in]   priority        interrupt priority
 *
 */
size_t timer_set_interval(timer_device_number_t timer_number, timer_channel_number_t channel, size_t nanoseconds);

/**
 * @brief       [DEPRECATED] Set timer timeout function
 *
 * @param[in]   timer           timer
 * @param[in]   channel         channel
 * @param[in]   func            timeout function
 * @param[in]   priority        interrupt priority
 *
 */
void timer_set_irq(timer_device_number_t timer_number, timer_channel_number_t channel, void (*func)(), uint32_t priority);

/**
 * @brief       Enable timer
 *
 * @param[in]   timer       timer
 * @param[in]   channel     channel
 * @param[in]   enable      Enable or disable
 *
 */
void timer_set_enable(timer_device_number_t timer_number, timer_channel_number_t channel, uint32_t enable);

void timer_enable_interrupt(timer_device_number_t timer_number, timer_channel_number_t channel);
void timer_disable_interrupt(timer_device_number_t timer_number, timer_channel_number_t channel);
//系统最小调度时间片
// 1ms
#define MIN_TIME_STEP   (10000000UL)

#define DEFAULT_TIME_STEP (MIN_TIME_STEP*7000)
#endif