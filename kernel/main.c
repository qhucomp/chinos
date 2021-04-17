#include "include/riscv.h"
#include "include/uart.h"
#include "include/platform.h"
#include "include/ecall.h"
#include "include/syscalls.h"
#include "include/printk.h"
#include "include/logo.h"

extern void _trap_entry(void);
void kernel_init(void) {
  write_mtvec((uint64_t)_trap_entry);
  printk("init kernel.........OK");
}
int main(void) {
    print_logo();
    kernel_init();
    while(1) {
      int ch =   uart_receive();
      if (ch != EOF) { 
          uart_send(ch);
      }
    }
    return 0;
}