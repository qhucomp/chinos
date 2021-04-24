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
extern void _trap_entry(void);
void timer_callback(void *ctx) {
  (void)ctx;
  printk("timer test!\n");
}
void kernel_init(void) {
  _write_mtvec((uint64_t)_trap_entry);
  init_kmalloc();
  plic_init();
  timer_init(TIMER_DEVICE_0);
  timer_set_interval(TIMER_DEVICE_0,TIMER_CHANNEL_0,100000000000);
  timer_set_irq(TIMER_DEVICE_0,TIMER_CHANNEL_0,timer_callback,1);
  timer_set_enable(TIMER_DEVICE_0,TIMER_CHANNEL_0,1);
  sysctl_enable_irq();
  //syscalls[64] = (syscall_func)sys_write;
  printk("init kernel.........OK\n");
}
int main(void) {
    kernel_init();
    print_logo();
    ECALL(0,0,0,0);
    while (1);
    return 0;
}