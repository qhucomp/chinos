#include "include/trap.h"
#include "include/kernel_thread.h"
#include "include/kmalloc.h"
#include "include/riscv.h"
#include "include/printk.h"
#include "include/task.h"

task_struct *kernel_thread(void *address) {
    task_struct *task = alloc_task();
    add_task(task);
    task->epc = (uintptr_t)address;
    printk("kernel pid:%d\n",task->pid);
    return task;
}