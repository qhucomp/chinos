#include "include/init.h"
#include "include/shell.h"
#include "include/printk.h"
#include "include/user.h"
#include "include/thread_test.h"
void task_init(void) {
    printk("task_init process..........OK\n");
    //user_thread(thread_test2);
    while(1);
}