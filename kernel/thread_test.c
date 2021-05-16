#include "include/thread_test.h"
#include "include/printk.h"
#include "include/riscv.h"
#include "include/encoding.h"
#include "include/sysctl.h"
#include "include/uart.h"
#include "include/diskio.h"
#include "include/ecall.h"
#include "include/syscalls.h"
#include "include/string.h"
void thread_test1(void) {
    char buf[512];
    memset(buf,0,512);
    char *path="testtesttest.txt";
    int fd = ECALL(SYS_openat,0,path,0);
    ECALL(SYS_read,fd,buf,512); 
    int result = strlen(buf);   
    while(1) {
        ECALL(SYS_write,1,buf,result);
    }
    while(1)
        printk("test!\n");
}

void thread_test2(void) {
    //char *output = "user thread test2\n";
    while(1) {
        printk("test");
        //ECALL(SYS_write,1,output,18);
    }
}