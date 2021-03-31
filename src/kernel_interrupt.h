#ifndef KERNEL_INTERRUPT
#define KERNEL_INTERRUPT
#include <stdint.h>
void handle_interrupt(uint64_t regs[32],uint64_t sepc,uint64_t sstatus);
#endif