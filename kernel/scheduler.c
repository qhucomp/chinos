#include "include/task.h"
#include "include/scheduler.h"
#include "include/timer.h"
#include "include/printk.h"
#include "include/sysctl.h"
#include "include/riscv.h"
#include "include/assert.h"
#include "include/thread_test.h"
uint64_t last_time_interrupt;
void init_scheduler(void) {
    current = &init_task;
    task_list = &init_task;
    //初始化时钟中断
    timer_init(TIMER_DEVICE_0);
    timer_set_interval(TIMER_DEVICE_0,TIMER_CHANNEL_0,DEFAULT_TIME_STEP);
    timer_set_irq(TIMER_DEVICE_0,TIMER_CHANNEL_0,task_scheduler,1);
    timer_set_enable(TIMER_DEVICE_0,TIMER_CHANNEL_0,1);
    printk("scheduler init........OK\n");
}

void task_scheduler(void *data) {
    (void)data;
    uint64_t t = sysctl_get_time_us() / 1000;
    uint64_t deltaT = t - last_time_interrupt;
    last_time_interrupt = t;
}