#ifndef __THREAD_H
#define __THREAD_H
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
//typedef uint32_t pid_t;

typedef struct __task_struct {
    pid_t pid;
    struct __task_struct *next;
    struct __task_struct *prev;
} task_struct;
extern task_struct init_task;
extern task_struct *task_list;
#define INIT_TASK(task)                     \
{                                           \
    .pid = 0,                               \
    .next = &init_task,                     \
    .prev = &init_task                      \
}
void add_task(task_struct *task);
void delte_task(task_struct *task);
task_struct *alloc_task(void);
extern task_struct *current;
extern uint64_t pid_bitmap[32];
#endif