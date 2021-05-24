#include "include/shell.h"
#include "include/user.h"
#include "include/printk.h"
#include "include/sysctl.h"
#include "include/syscalls.h"
#include "include/ecall.h"
int task_count;
int user_shell(void) {
    const char run_list[][15] ={"/read","/fork","/brk","/clone","/mmap","/munmap",
                                "/dup","/write ","/getcwd","/getpid","/gettimeofday",
                                "/mount","/open","/pipe","/test_echo","/times","/uname",
                                "/wait","/yield_B","/chdir","/close","/dup2","/exit",
                                "/fstat","/getdents","/mkdir_","/openat","/sleep",
                                "/umount","/unlink","/waitpid","/yield_A","/yield_C"};
    
    // user_thread(run_list[0]);
    //user_thread(run_list[7]);
    // printk("i:%d\n",task_count);
    // if (task_count < 3) {
    //     printk("ecall\n");
    //     ECALL(SYS_user_task,run_list[task_count++],0,0,0,0,0);
    // } else {
    //     task_count = 0;
    // }
    //ECALL(SYS_user_task,run_list[7],0,0,0,0,0);
    //asm volatile("nop;");
    //printk("exit code!\n");
    // int status;
    // ECALL(SYS_wait4,-1,&status,0,0,0,0);
    // printk("status:%d\n",status);
    // for(int i = 1;i < 38;i++)
    //     user_thread(run_list[i]);
    // printk("run end...\n");
    // printk("count:%d\n",task_count);
    // if (task_count == 0) {
    //     task_count++;
        int status;

        ECALL(SYS_user_task,run_list[0],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[1],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[2],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        printk("wait\n");
        ECALL(SYS_user_task,run_list[17],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);


        // ECALL(SYS_wait4,-1,&status,0,0,0,0);
        //printk("read\n");
        // printk("wait\n");
        printk("test code\n");
    // }
    while(1);
    return 1;
}