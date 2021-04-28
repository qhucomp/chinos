#include "include/task.h"
#include "include/scheduler.h"
#include "include/timer.h"
#include "include/printk.h"
#include "include/sysctl.h"
#include "include/riscv.h"
#include "include/assert.h"
#include "include/thread_test.h"
#include "include/idle.h"
uint64_t last_time_interrupt;
void init_scheduler(void) {
    current = &init_task;
    current->epc = (uintptr_t)idle;
    current->create_time = sysctl_get_time_us() / 1000;
    current->left_time = 5000;
    current->pid = 0;
    pid_bitmap[0] = 1;
    task_list = current;
    task_list->next = task_list->prev = task_list;

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

    if (deltaT >= current->left_time) {
        current->left_time = 5000;
        task_struct *next_task = task_list->next;
        task_list->next = NULL;
        task_list->prev->next = task_list;
        next_task->prev = task_list;
        task_list = next_task;
        current = task_list;
    } else {
        current->left_time -= deltaT;
    }
}