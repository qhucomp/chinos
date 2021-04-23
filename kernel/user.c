#include "include/user_test.h"
#include "include/ecall.h"
#include "include/printk.h"
void user_test(void) {
    printk("WOCAO");
    //ECALL(64,1,c,12);
    while(1);
}