#include "include/riscv.h"
#include "include/uart.h"
#include "include/platform.h"
#include "include/ecall.h"
#include "include/syscalls.h"
#include "include/printk.h"
#include "include/logo.h"
#include "include/kmalloc.h"

extern void _trap_entry(void);
void kernel_init(void) {
  write_mtvec((uint64_t)_trap_entry);
  init_kmalloc();
  printk("init kernel.........OK\n");
}
int main(void) {
    kernel_init();
    print_logo();
    char *ptr = kmalloc(1024);
    printk("address:%p\n",ptr);
    while(1) {
      int ch =   uart_receive();
      if (ch != EOF) { 
          uart_send(ch);
      }
    }
    return 0;
}