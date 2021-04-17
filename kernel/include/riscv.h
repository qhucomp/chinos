#ifndef __RISCV_H
#define __RISCV_H
#include <stdint.h>
#define _read_mcause()                                      \
({                                                          \
    register unsigned long a0 asm("a0") = 0;                \
    asm volatile("csrr %0,mcause":"+r"(a0)::"memory");      \
    a0;                                                     \
})
#define _read_mepc()                                        \
({                                                          \
    register unsigned long a0 asm("a0") = 0;                \
    asm volatile("csrr %0,mepc":"+r"(a0)::"memory");        \
    a0;                                                     \
})
#define _write_mepc(epc)                                            \
({                                                                  \
    register unsigned long a0 asm("a0") = (unsigned long)epc;       \
    asm volatile("csrw mepc,%0"::"r"(a0):"memory");                 \
})
#define _write_mtvec(entry)                                         \
({                                                                  \
    register unsigned long a0 asm("a0")= (unsigned long)entry;      \
    asm volatile("csrw mtvec,%0"::"r"(a0));                         \
})
#define _read_mstatus()                                             \
({                                                                  \
    register unsigned long a0 asm("a0") = (unsigned long)0;         \
    asm volatile("csrr %0,mstatus":"+r"(a0)::"memory");             \
    a0;                                                             \
})
#define _write_mstatus(mstatus)                                     \
({                                                                  \
    register unsigned long a0 asm("a0") = (unsigned long)mstatus;   \
    asm volatile("csrr %0,mstatus"::"r"(a0):"memory");             \
})
#define MPP_MACHINE     (3 << 11)
#define MPP_HYPERVISOR  (2 << 11)
#define MPP_SUPERVISOR  (1 << 11)
#define MPP_USER        (0)

#define CAUSE_USER_ECALL        (0X8)
#define CAUSE_SUPERVISOR_ECALL  (0x9)
#define CAUSE_HYPERVISOR_ECALL  (0xa)
#define CAUSE_MACHINE_ECALL     (0xb)

void set_mpp(uint64_t mpp);
uint64_t inline read_mcause(void) {return _read_mcause();}
uint64_t inline read_mepc(void) {return _read_mepc();}
uint64_t inline read_mstatus(void) {return _read_mstatus();}
void inline write_mtvec(uint64_t a0) {_write_mtvec(a0);}
void inline write_mepc(uint64_t a0) {_write_mepc(a0);}
void inline write_mstatus(uint64_t a0) {_write_mstatus(a0);}
#endif