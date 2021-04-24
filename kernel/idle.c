#include "include/idle.h"
#include "include/ecall.h"
#include "include/printk.h"
void idle(void) {
    printk("idle init..........OK\n");
    while(1);
}