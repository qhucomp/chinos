#include "include/task.h"
#include "include/scheduler.h"
#include "include/timer.h"
#include "include/printk.h"
#include "include/sysctl.h"
#include "include/riscv.h"
#include "include/assert.h"
#include "include/thread_test.h"
#include "include/idle.h"
#include "include/init.h"
#include "include/kernel_thread.h"
#include "include/user.h"
#include "include/shell.h"
#include "include/string.h"
#include "include/kmalloc.h"
uint64_t last_time_interrupt;
void init_scheduler(void) {
    current = &init_task;
    current->epc = (uintptr_t)idle;
    current->create_time = sysctl_get_time_us() / 1000;
    current->left_time = 1000;
    current->pid = 0;
    current->fd_bitmap = 1;
    current->entry = kmalloc(sizeof(dentry_struct *) * 64);
    memset(current->entry,0,sizeof(dentry_struct *) * 64);
    pid_bitmap[0] = 1;
    task_list = current;
    task_list->next = task_list->prev = task_list;
    current->work_dir = kmalloc(256);
    if(!current->work_dir)
        panic("out of memory!");
    memset(current->work_dir,0,256);
    memcpy(current->work_dir,"/riscv64/",8);
    //kernel_thread(user_shell);
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
        printk("scheduler!\n");
        current->left_time = 1000;
        task_struct *next_task = current->next;
        current->status = TASK_WAIT_CPU;
        if (!next_task)
            next_task = task_list;

        current = next_task;
        current->status = TASK_RUN;
        if (current->flag & TASK_FLAG_USER_THREAD)
            set_mpp(MPP_USER);
        else
            set_mpp(MPP_MACHINE);

        printk("current pid:%d flag:%d\n",current->pid,next_task->flag);
    } else {
        current->left_time -= deltaT;
    }
}