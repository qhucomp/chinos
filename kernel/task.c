#include "include/task.h"
#include "include/kmalloc.h"
#include "include/sysctl.h"
#include "include/printk.h"
task_struct init_task = INIT_TASK(init_task);
task_struct *task_list;
uint64_t pid_bitmap[32];
task_struct *current;
void add_task(task_struct *task) {
    //RR调度算法
    sysctl_disable_irq();
    printk("add_task():pid:%d bool:%d\n",task->pid,task_list->next == task_list);
    if (task_list->next == task_list) {
        task_list->next = task;
        printk("task pid:%d\n",task_list->next->pid);
        task_list->prev = task;
        task->prev = task_list;
        task->next = NULL;
    } else {
        task_struct *last_task = task_list->prev;
        last_task->next = task;
        task->prev = last_task;
        task_list->prev = task;
        task->next = NULL;
    }
    sysctl_enable_irq();
}

static void clear_pid(pid_t pid) {
    int i,j;
    i = pid % 64;
    if (i > 0)
        j = i*64 - pid;
    else
        j = pid;

    pid_bitmap[i] &= ~(1 << j);
}

void delete_task(task_struct *task) {
    //RR调度算法
    sysctl_disable_irq();
    clear_pid(task->pid);
    if (task->next == NULL) {
        //删除队列末尾的task_struct
        task->prev->next = NULL;
        task_list->prev = task->prev;
        kfree(task);
    } else if (task->prev->next == NULL) {
        //删除队首
        task_struct *next_task = task->next;
        next_task->prev = task->prev;
        task_list = next_task;
        kfree(task); 
    } else {
        //删除队列中间项
        task_struct *prev_task = task->prev;
        task_struct *next_task = task->next;
        prev_task->next = next_task;
        next_task->prev = prev_task;
        kfree(task);
    }
    sysctl_enable_irq();
}

static pid_t get_new_pid(void) {
    for(int i = 0;i < 32;i++) {
        if (~(pid_bitmap[i] & 0xffffffffffffffff))
            for(int j = 0;j < 64;j++)
                if (!(pid_bitmap[i] & (1 << j))) {
                    pid_bitmap[i] |= 1 << j;
                    return i + j;
                }
    }
    return 0;
}

task_struct *alloc_task(void) {
    pid_t pid = get_new_pid();
    task_struct *task = kmalloc(sizeof(task_struct));
    task->pid = pid;
    task->next = task->prev = NULL;
    task->left_time = 5000;
    task->create_time = sysctl_get_time_us() / 1000;
    task->epc = 0;
    task->flag = 0;
    return task;
}