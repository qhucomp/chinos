#ifndef __USER_THREAD_H
#define __USER_THREAD_H
#include "trap.h"
typedef struct __user_thread {
    regs *reg;
    uintptr_t epc;
} user_thread;

user_thread *create_thread(void *address);
void start_thread(user_thread *thread);
#endif