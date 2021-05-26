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
#include "include/init.h"
#include "include/string.h"
#include "include/elf_parse.h"
#include "include/task.h"

extern void _trap_entry(void);
void kernel_init(void) {
    write_csr(mtvec,_trap_entry);
    // 使用6m以上的内存
    // sysctl_pll_set_freq(SYSCTL_PLL1,800000000UL);
    // sysctl_pll_enable(SYSCTL_PLL1);
    fpioa_pin_init();
    init_kmalloc();
    plic_init();
    disk_init();
    fat32_init();
    //register_syscall();
    init_scheduler();

    //开启中断
    sysctl_enable_irq();
    last_time_interrupt = sysctl_get_time_us() / 1000;
    printk("init kernel.........OK\n");
    print_logo();
}

int main(void) {
    kernel_init();
    ECALL(0,0,0,0,0,0,0);
    while (1);
    return 0;
}