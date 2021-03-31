#include "kernel_interrupt.h"
#include <stdio.h>
void handle_interrupt(uint64_t regs[32],uint64_t sepc,uint64_t sstatus) {
    printf("handle!\n");
} 