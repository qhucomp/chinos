#ifndef __KERNEL_THREAD_H
#define __KERNEL_THREAD_H
#include "trap.h"
#include "task.h"
task_struct *kernel_thread(void *address);
#endif