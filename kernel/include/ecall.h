#ifndef __ECALL_H
#define __ECALL_H
#define ECALL(__num, __a0, __a1, __a2,__a3,__a4,__a5)                                                   \
({                                                                                                      \
    register int64_t a0 asm("a0") = (int64_t)(__a0);                                                    \
    register int64_t a1 asm("a1") = (int64_t)(__a1);                                                    \
    register int64_t a2 asm("a2") = (int64_t)(__a2);                                                    \
    register int64_t a3 asm("a3") = (int64_t)(__a3);                                                    \
    register int64_t a4 asm("a4") = (int64_t)(__a4);                                                    \
    register int64_t a5 asm("a5") = (int64_t)(__a5);                                                    \
    register int64_t a7 asm("a7") = (unsigned long)(__num);                                             \
    asm volatile("ecall" : "+r"(a0) :"r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a7) : "memory");  \
    a0;                                                                                                 \
})
#endif