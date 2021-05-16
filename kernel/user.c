#include "include/user.h"
#include "include/riscv.h"
#include "include/task.h"
#include "include/vfs.h"
#include "include/printk.h"
#include "include/string.h"
int f;
void user_thread(const char *name) {
    task_struct *task = alloc_task();
    printk("task->pid=%d\n",task->pid);
    void *user_space = user_malloc(task->pid);
    memset(user_space,0,USER_HEAP_SIZE);
    printk("user_space:%p\n",user_space);
    dentry_struct *p = vfs_open(name);
    printk("vfs_open OK!\n");
    vfs_read(p,user_space,p->file_size);    
    printk("instruction\n");
    for(uint64_t *ptr = user_space;(uintptr_t)ptr < (uintptr_t)user_space + 8192;ptr++) {
        printk("%p ",*ptr);
    }
    printk("\n");
    task->epc = (uintptr_t)(user_space);
    printk("user epc:%p\n",task->epc);
    task->flag |= TASK_FLAG_USER_THREAD;
    task->task_reg.x11 = task->sp = (uintptr_t)(user_space + p->file_size + 512*5);
    add_task(task);
    printk("user sp:%p\n",task->pid,task->sp);
}

void *user_malloc(pid_t pid) {
    uintptr_t user_process_address = 0x80400000 + 4096;
    return (char *)(user_process_address + (pid - 2) * USER_HEAP_SIZE);
}