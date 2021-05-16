#include "include/shell.h"
#include "include/user.h"
#include "include/printk.h"
#include "include/sysctl.h"
int user_shell(void) {
    printk("read read.bin\n");
    sysctl_disable_irq();
    user_thread("/read.bin");
    sysctl_enable_irq();
    while(1);
    return 1;
}