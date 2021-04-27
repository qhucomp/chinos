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
    timer_set_interval(TIMER_DEVICE_0,TIMER_CHANNEL_0,MIN_TIME_STEP*1000);
    timer_set_irq(TIMER_DEVICE_0,TIMER_CHANNEL_0,task_scheduler,1);
    timer_set_enable(TIMER_DEVICE_0,TIMER_CHANNEL_0,1);
    printk("scheduler init........OK\n");
}

void task_scheduler(void *data) {
    (void)data;
    uint64_t t = sysctl_get_time_us() / 1000;
    //uint64_t deltaT = t - last_time_interrupt;
    //临时调度器
    if (current == task1) {
        current = task2;
        //current->epc = (unsigned long)thread_test2;
    }
    else {
        current = task1;
        //current->sp = 0x80600000;
        //current->epc = (unsigned long)thread_test1;
    }
    printk("pid:%d epc:%p\n",current->pid,current->epc);
    //_write_mepc(current->epc);
    /*if (deltaT > current->left_time) {
        task_struct *task = current;
        current = task_list->next;
        printk("prev_pid:%d pid:%d next_pid:%d\n",task->pid,current->pid,current->next->pid);
        assert(current);
        task->next = current->next;
        current->next->prev = task;
        task->prev = current;
        current->next = task;
        current->prev = NULL;
        current->left_time = TASK_DEFAULT_TIME_STEP;
        task->left_time = TASK_DEFAULT_TIME_STEP;
        task->next = current->next;
        current->next = task;

    } else {
        current->left_time -= deltaT;
        _write_mepc(current->epc);
    }*/
    last_time_interrupt = t;
}