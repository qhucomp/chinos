#include "include/idle.h"
#include "include/ecall.h"
#include "include/printk.h"
void idle(void) {
    while(1)
        printk("idle init..........OK\n");
}