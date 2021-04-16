#ifndef __RISCV_H
#define __RISCV_H
#include <stdint.h>
#define read_mcause()                                       \
({                                                          \
    register unsigned long a7 asm("a7") = 0;                \
    asm volatile("csrr %0,mcause":"+r"(a7)::"memory");      \
    a7;                                                     \
})
#define read_mepc()                                         \
({                                                          \
    register unsigned long a7 asm("a7") = 0;                \
    asm volatile("csrr %0,mepc":"+r"(a7)::"memory");        \
    a7;                                                     \
})
#define write_mepc(epc)                                             \
({                                                                  \
    register unsigned long a7 asm("a7") = (unsigned long)epc;       \
    asm volatile("csrw mepc,%0"::"r"(a7):"memory");                 \
})
#define write_mtvec(entry)                                          \
({                                                                  \
    register unsigned long a0 asm("a0")= (unsigned long)entry;      \
    asm volatile("csrw mtvec,%0"::"r"(a0));                         \
})
#endif