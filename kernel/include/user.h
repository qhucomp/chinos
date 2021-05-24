#ifndef __USER_H
#define __USER_H
#include <sys/types.h>
#include "task.h"

/**
 * @brief 创建一个用户线程
 * 
 * @param[in] address 用户线程的地址
 * 
 * @param[in] sp 栈地址
 */
task_struct *user_thread(const char *name);

/**
 * @brief 给用户进程分配空间
 * 
 * @param[in] pid 进程pid
 * 
 * @return 返回地址或者NULL
 */
void *user_malloc(pid_t pid);
#define get_user_space(pid) user_malloc(pid)
#define USER_HEAP_SIZE (512*1024)
#endif