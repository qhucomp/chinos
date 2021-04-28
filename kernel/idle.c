#include "include/idle.h"
#include "include/ecall.h"
#include "include/printk.h"

//idle进程
//运行在MACHINE态
void idle(void) {
    printk("idle init..........OK\n");
    while(1);
}