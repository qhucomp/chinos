#ifndef __SCHEDULER_H
#define __SCHEDULER_H
#include <stdint.h>

extern uint64_t last_time_interrupt; 
//初始化调度器
void init_scheduler(void);

//执行任务调度
void task_scheduler(void *data);
#endif