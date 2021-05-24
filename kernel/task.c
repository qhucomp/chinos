#include "include/task.h"
#include "include/kmalloc.h"
#include "include/sysctl.h"
#include "include/printk.h"
#include "include/string.h"
task_struct idle_task;
task_struct *task_list;
uint64_t pid_bitmap[32];
task_struct *current;
void add_task(task_struct *task) {
    //RR调度算法
    //sysctl_disable_irq();
    // printk("add_task():pid:%d bool:%d\n",task->pid,task_list->next == task_list);
    if (task_list->next == task_list) {
        task_list->next = task;
        //printk("task pid:%d\n",task_list->next->pid);
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
    //sysctl_enable_irq();
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
    //sysctl_disable_irq();

    //释放子进程
    for(uint32_t i = 0;i < task->chilren_count;i++)  {
        if (task->chilren[i]) {
            delete_task(task->chilren[i]);
            kfree(task->chilren[i]);
        }
    }
    //释放打开的文件
    for(uint32_t i = 0;i < 64;i++)  {
        if (task->fd_bitmap & (1ULL << i)) {
            free_dentry(task->entry[i]);
        }
    }
    kfree(task->chilren);
    kfree(task->work_dir);
    kfree(task->entry);
    clear_pid(task->pid);
    task->status = TASK_DIE;
    if (task->next == NULL) {
        //删除队列末尾的task_struct
        task->prev->next = NULL;
        task_list->prev = task->prev;
    } else if (task->prev->next == NULL) {
        //删除队首
        task_struct *next_task = task->next;
        next_task->prev = task->prev;
        task_list = next_task;
    } else {
        //删除队列中间项
        task_struct *prev_task = task->prev;
        task_struct *next_task = task->next;
        prev_task->next = next_task;
        next_task->prev = prev_task;
    }
    //sysctl_enable_irq();
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
void init_task(pid_t pid,task_struct *task,task_struct *parent) {
    if (!task)
        panic("out of memory!");

    task->pid = pid;
    task->next = task->prev = NULL;
    task->left_time = 1000;
    task->create_time = sysctl_get_time_us() / 1000;
    memset(&task->task_reg,0,sizeof(regs));
    task->epc = 0;
    task->flag = TASK_FLAG_NO_RUN;
    task->status = TASK_WAIT_CPU;
    task->fd_bitmap = 1;
    task->entry = kmalloc(sizeof(dentry_struct *) * 64);
    if (!task->entry)
        panic("out of memory!");
    memset(task->entry,0,sizeof(dentry_struct *) * 64);

    task->work_dir = kmalloc(256);
    if(!task->work_dir)
        panic("out of memory!");
    memset(task->work_dir,0,256);

    if (parent != NULL)
        memcpy(task->work_dir,parent->work_dir,strlen(parent->work_dir));
    else
        memcpy(task->work_dir,"/",1);
    task->parent = parent;
    for(int i = 0;i < parent->chilren_len;i++)
        if(!task->chilren[i])
            task->chilren[i] = task;
    task->chilren = kmalloc(sizeof(task_struct *) * 8);
    if(!task->chilren)
        panic("out of memory!");
    memset(task->chilren,0,sizeof(task_struct *) * 8);

    task->chilren_len = 8;
}
task_struct *alloc_task(task_struct *parent) {
    pid_t pid = get_new_pid();
    task_struct *task = kmalloc(sizeof(task_struct));
    if (!task)
        panic("out of memory!");
    init_task(pid,task,parent);
    return task;
}