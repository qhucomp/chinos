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
        
        int status;
        printk("start test\n");
        ECALL(SYS_user_task,run_list[0],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[1],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[2],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[4],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[9],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[23],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[17],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[16],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[7],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[22],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[12],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);

        ECALL(SYS_user_task,run_list[30],0,0,0,0,0);
        ECALL(SYS_wait4,-1,&status,0,0,0,0);
        printk("end test\n");
    while(1);
    return 1;
}