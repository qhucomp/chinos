#include "include/riscv.h"
#include "include/uart.h"
#include "include/platform.h"
#include "include/ecall.h"
#include "include/syscalls.h"
#include "include/printk.h"
#include "include/logo.h"
#include "include/kmalloc.h"
#include "include/idle.h"
#include "include/kernel_thread.h"
#include "include/timer.h"
#include "include/sysctl.h"
#include "include/plic.h"
#include "include/scheduler.h"
#include "include/thread_test.h"
extern void _trap_entry(void);
void kernel_init(void) {
    _write_mtvec((uint64_t)_trap_entry);
    init_kmalloc();
    plic_init();

    //调度器初始化
    init_scheduler();

    //创建两个内核线程
    kernel_thread(thread_test1)->sp = 0x80600000;
    kernel_thread(thread_test2)->sp = 0x80500000;

    //开启中断
    sysctl_enable_irq();
    last_time_interrupt = sysctl_get_time_us() / 1000;
    printk("init kernel.........OK\n");
    print_logo();
}
int main(void) {
    kernel_init();
    ECALL(0,0,0,0);
    while (1);
    return 0;
}