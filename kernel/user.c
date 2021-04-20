#include "include/user_test.h"
#include "include/ecall.h"
void user_test(void) {
    char *c = "thread test!\n";
    ECALL(64,1,c,12);
    while(1);
}