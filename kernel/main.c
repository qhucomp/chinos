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
extern void _trap_entry(void);
void _m_mode_start(void) {
    //printk("m mode\n");
    asm volatile(
    ".section .text\n"
    "1:"
    "    auipc ra,%pcrel_hi(1f);"
    "    ld ra,%pcrel_lo(1b)(ra);"
    "    ret;"
    "    jr ra;"
    ".align 3\n"
    "1:  .dword 0x80301000"
    );
    printk("process failed\n");
}
void kernel_init(void) {
    write_csr(mtvec,_trap_entry);
    //_write_mtval((uint64_t)_trap_entry);
    // 使用6m以上的内存
    sysctl_pll_set_freq(SYSCTL_PLL1,800000000UL);
    sysctl_pll_enable(SYSCTL_PLL1);

    init_kmalloc();
    plic_init();
    disk_init();
    fat32_init();
    //register_syscall();
    //开启中断
    sysctl_enable_irq();
    last_time_interrupt = sysctl_get_time_us() / 1000;
    printk("init kernel.........OK\n");
    print_logo();
}

int main(void) {
    //asm volatile("mv sp,%0"::"r"(0x80200000));
    kernel_init();
    dentry_struct *p = fat32_open("/riscv64/read");
    void *user = (char *)(0x80300000);
    memset(user,0,5120);
    void *elf = kmalloc(128*1024);
    fat32_read(p,elf,128*1024);

    copy_section(elf,user,".text");
    copy_section(elf,user,".rodata");
    copy_section(elf,user,".data");
    copy_section(elf,user,".srodata"); 
    init_scheduler();
    write_csr(mepc,_m_mode_start);
    asm volatile("mret");
    while (1);
    return 0;
}