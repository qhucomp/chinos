#include "include/trap.h"
#include "include/user_thread.h"
#include "include/kmalloc.h"
#include "include/riscv.h"
#include "include/printk.h"

user_thread *create_thread(void *address) {
    user_thread *thread = kmalloc(sizeof(user_thread));
    regs *reg = kmalloc(sizeof(regs));
    if ((thread == NULL) || (reg == NULL)) {
        printk("OOM!\n");
        return NULL;
    }

    thread->reg = reg;
    thread->reg->x2 = 0x80400000;
    thread->epc = (uintptr_t)address;
    return thread;
}

void start_thread(user_thread *thread) {
    printk("call thread");
    register uint64_t a0 asm("a0") = (uint64_t)thread->reg;
    _write_mepc(thread->epc);
    _write_mstatus(MPP_SUPERVISOR);
    asm volatile("mv sp,%0"::"r"(a0):"memory");
    asm volatile("j _restore");
}