#ifndef __KERNEL_THREAD_H
#define __KERNEL_THREAD_H
#include "trap.h"
#include "task.h"
/**
 * @brief   启动一个内核线程
 * 
 * @param[in] address   内核线程地址
*/
task_struct *kernel_thread(void *address);
#endif