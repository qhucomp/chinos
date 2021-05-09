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
#include "include/diskio.h"
#include "include/fpioa.h"
#include "include/gpiohs.h"
#include "include/sdcard.h"
#include "include/diskio.h"
#include "include/fat32.h"
#include "include/encoding.h"
extern void _trap_entry(void);
void kernel_init(void) {
    _write_mtvec((uint64_t)_trap_entry);
    // 使用6m以上的内存
    sysctl_pll_set_freq(SYSCTL_PLL1,800000000UL);
    sysctl_pll_enable(SYSCTL_PLL1);

    init_kmalloc();
    plic_init();
    //调度器初始化
    clear_csr(mie, MIP_MEIP);
    clear_csr(mstatus, MSTATUS_MIE);


    //printk("task task\n");
    //init_scheduler();
    //kernel_thread(thread_test1)->sp = 0x80600000;
    //kernel_thread(thread_test2)->sp = 0x80500000;
    //创建两个内核线程
    printk("/******************sdcard test*****************/\n");
    disk_init();
    fat32_init();

    //开启中断
    sysctl_enable_irq();
    last_time_interrupt = sysctl_get_time_us() / 1000;
    printk("init kernel.........OK\n");
    print_logo();
}
int main(void) {
    asm volatile("mv sp,%0"::"r"(0x80200000));
    kernel_init();
    ECALL(0,0,0,0);
    while (1);
    return 0;
}