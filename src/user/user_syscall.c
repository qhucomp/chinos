#include "user_syscall.h"

void syscall(int syscall_no, uint64_t param1, uint64_t param2, uint64_t param3,
              uint64_t param4, uint64_t param5, uint64_t param6) {
  uint64_t load_regs[8] = {0,syscall_no, param1, param2,param3, param4,param5, param6};  //通过栈把值弹到寄存器里
  uint64_t save_regs[8];
  uint64_t ptr = 0;
  uint64_t ret[2];
  printf("syscall\n");

    ptr = 0;
    /*asm volatile(
        "addi sp,sp,-8*8\n"
        "sd x10,1*8(sp)\n"
        "sd x11,2*8(sp)\n"
        "sd x12,3*8(sp)\n"
        "sd x13,4*8(sp)\n"
        "sd x14,5*8(sp)\n"
        "sd x15,6*8(sp)\n"
        "sd x17,7*8(sp)\n");*/

    asm volatile(
        "sd sp,(%0)\n"
        "sd x10,1*8(%0)\n"
        "sd x11,2*8(%0)\n"
        "sd x12,3*8(%0)\n"
        "sd x13,4*8(%0)\n"
        "sd x14,5*8(%0)\n"
        "sd x15,6*8(%0)\n"
        "sd x17,7*8(%0)\n"::"r"(save_regs));

    asm volatile(
        "ld x17,1*8(%0);"
        "ld x10,2*8(%0);"
        "ld x11,3*8(%0);"
        "ld x12,4*8(%0);"
        "ld x13,5*8(%0);"
        "ld x14,6*8(%0);"
        "ld x15,7*8(%0);"::"r"(load_regs));
    //asm volatile("csrr a0,mepc;addi a0,a0,16");
    asm volatile("ecall;");  //调用系统调用
    //恢复现场
    /*asm volatile(
        "ld x11,2*8(sp)\n"
        "ld x12,3*8(sp)\n"
        "ld x13,4*8(sp)\n"
        "ld x14,5*8(sp)\n"
        "ld x15,6*8(sp)\n"
        "ld x17,7*8(sp)\n");*/

    asm volatile(
        //"ld x10,1*8(%0)\n"
        "ld sp,(%0)\n"
        "ld x11,2*8(%0)\n"
        "ld x12,3*8(%0)\n"
        "ld x13,4*8(%0)\n"
        "ld x14,5*8(%0)\n"
        "ld x15,6*8(%0)\n"
        "ld x17,7*8(%0)\n"::"r"(save_regs));
    asm volatile("sd x10,(%0)" ::"r"(ret));  // x10寄存器用作返回值
    //asm volatile("ld x10,1*8(%0)":"=r"(save_regs));  // 恢复0寄存器用作返回值
    asm volatile("addi sp,sp,8*8");
    //asm volatile("mv x10,%0"::"r"(10));
    //return (void *)ret;
}
