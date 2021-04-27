#ifndef __SCHEDULER_H
#define __SCHEDULER_H
#include <stdint.h>

//上次中断的时间
extern uint64_t last_time_interrupt; 
/**
 * @brief 进程调度器初始化
 */
void init_scheduler(void);

/**
 * @brief 执行任务调度
 * @param[in] data 无作用
 */
void task_scheduler(void *data);
#endif