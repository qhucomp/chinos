#include "include/thread_test.h"
#include "include/printk.h"
#include "include/riscv.h"
#include "include/encoding.h"
#include "include/sysctl.h"
#include "include/uart.h"
#include "include/diskio.h"
task_struct *task1;
task_struct *task2;
void sysctl_disable_irq(void)
{
    clear_csr(mie, MIP_MEIP);
    clear_csr(mstatus, MSTATUS_MIE);
}
void thread_test1(void) {
    unsigned long sp;
    asm volatile("mv %0,sp":"=r"(sp));
    char *test1 = "thread_test1";
    test1[13] = 0;
    while(1) {
        sysctl_disable_irq();
        uint8_t buf[512];
        disk_read(0,buf,0,1);
        for(int i = 0;i < 512;i++)
            printk("%x ",buf[i]);
        //printk("sp:%p %p %s\n",sp,test1,test1);
        sysctl_enable_irq();
    }
}

void thread_test2(void) {
    unsigned long sp;
    asm volatile("mv %0,sp":"=r"(sp));
    char *test2 = "thread_test2";
    while(1) {
        sysctl_disable_irq();
        printk("sp:%p %p %s\n",sp,test2,test2);
        sysctl_enable_irq();

    }
}