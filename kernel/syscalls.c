#include <stdint.h>
#include "include/trap.h"
#include "include/syscalls.h"
#include "include/uart.h"
uintptr_t handle_ecall(uint64_t extension,regs *reg) {
    switch(extension) {
        case SYS_write:
            return sys_write((int)reg->x11,(void *)reg->x12,reg->x13);
        default:
            return 0;
    }
}

static void putchar(char ch) {
    uart_send(ch);
}

ssize_t sys_write(int fd,void *buf,size_t count) {
    if (fd == 1) {
        for(size_t i = 0;i < count;i++)
            putchar(((char *)buf)[i]);
    }
    return count;
}