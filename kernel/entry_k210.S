    .section .text.entry
    .globl _start
_start:
    add t0, a0, 1
    slli t0, t0, 14
	la sp, kernel_stack
    add sp, sp, t0
    call main

loop:
    j loop

    .section .bss.stack
    .align 12
    .globl kernel_stack
kernel_stack:
    .space 4096 * 4 * 2
    .globl kernel_stack_top
kernel_stack_top: