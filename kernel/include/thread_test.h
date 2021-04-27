#ifndef __THREAD_TEST_H
#define __THREAD_TEST_H
#include "task.h"
void thread_test1(void);
void thread_test2(void);
extern task_struct *task1;
extern task_struct *task2;
#endif