#include "include/idle.h"
#include "include/ecall.h"
#include "include/printk.h"
#include "include/user.h"

//idle进程
//运行在MACHINE态
void idle(void) {
    //printk("idle:%p\n",idle);
    //printk("idle init..........OK\n");
    user_thread("/read");
    while(1);
}