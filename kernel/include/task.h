#ifndef __THREAD_H
#define __THREAD_H
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include "timer.h"
#include "trap.h"
//typedef uint32_t pid_t;

//10毫秒
#define TASK_DEFAULT_TIME_STEP (10) 

typedef struct __task_struct {
    pid_t pid;
    uint64_t create_time;
    int64_t left_time;
    unsigned long epc;
    unsigned long sp;
    regs task_reg;
    int flag;
    struct __task_struct *next;
    struct __task_struct *prev;
} task_struct;
extern task_struct init_task;
extern task_struct *task_list;
#define INIT_TASK(task)                     \
{                                           \
    .pid = 0,                               \
    .create_time = 0,                       \
    .left_time = TASK_DEFAULT_TIME_STEP,    \
    .epc = 0,                               \
    .sp = 0,                                \
    .flag = 0,                              \
    .next = &init_task,                     \
    .prev = &init_task                      \
}
void add_task(task_struct *task);
void delte_task(task_struct *task);
task_struct *alloc_task(void);
extern task_struct *current;
extern uint64_t pid_bitmap[32];
#endif