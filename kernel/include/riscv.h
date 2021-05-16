#ifndef __RISCV_H
#define __RISCV_H
#include <stdint.h>
#define read_csr(reg) ({int64_t __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })
#define clear_csr(reg, bit) ({ unsigned long __tmp; \
  if (__builtin_constant_p(bit) && (unsigned long)(bit) < 32) \
    asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "i"(bit)); \
  else \
    asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  if (__builtin_constant_p(val) && (unsigned long)(val) < 32) \
    asm volatile ("csrw " #reg ", %0" :: "i"(val)); \
  else \
    asm volatile ("csrw " #reg ", %0" :: "r"(val)); })
#define set_csr(reg, bit) ({ unsigned long __tmp; \
  if (__builtin_constant_p(bit) && (unsigned long)(bit) < 32) \
    asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "i"(bit)); \
  else \
    asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
  __tmp; })
#define read_time() read_csr(mtime)
#define read_cycle() read_csr(mcycle)
#define current_coreid() read_csr(mhartid)

#define _read_mcause()                                      \
({                                                          \
    register long long a0 asm("a0") = 0;                    \
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
    asm volatile("csrr %0,mstatus"::"r"(a0):"memory");              \
})

#define MPP_MACHINE     (3UL << 11)
#define MPP_HYPERVISOR  (2UL << 11)
#define MPP_SUPERVISOR  (1UL << 11)
#define MPP_USER        (~MPP_MACHINE)

#define CAUSE_USER_ECALL        (0X8)
#define CAUSE_SUPERVISOR_ECALL  (0x9)
#define CAUSE_HYPERVISOR_ECALL  (0xa)
#define CAUSE_MACHINE_ECALL     (0xb)

/**
 * @brief 设置mstatus的mpp字段
 * 
 * @param[in] mpp mpp字段的值
 */
void set_mpp(uint64_t mpp);
#endif