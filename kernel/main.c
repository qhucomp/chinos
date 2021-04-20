#include "include/riscv.h"
#include "include/uart.h"
#include "include/platform.h"
#include "include/ecall.h"
#include "include/syscalls.h"
#include "include/printk.h"
#include "include/logo.h"
#include "include/kmalloc.h"
#include "include/user_test.h"
#include "include/user_thread.h"
extern void _trap_entry(void);
void kernel_init(void) {
  write_mtvec((uint64_t)_trap_entry);
  init_kmalloc();
  //syscalls[64] = (syscall_func)sys_write;
  printk("init kernel.........OK\n");
}
int main(void) {
    kernel_init();
    print_logo();
    user_thread *thread = create_thread(user_test);
    start_thread(thread);
    printk("failed\n");
    while (1);
    return 0;
}