#include "include/shell.h"
#include "include/user.h"
#include "include/printk.h"
#include "include/sysctl.h"
#include "include/syscalls.h"
#include "include/ecall.h"
extern uint64_t task_count;
int user_shell(void) {
    const char run_list[][15] ={"/read","/fork","/brk","/clone","/mmap","/munmap",
                                "/dup","/write ","/getcwd","/getpid","/GETTIM~1",
                                "/mount","/open","/pipe","/test_echo","/times","/uname",
                                "/wait","/yield_B","/chdir","/close","/dup2","/exit",
                                "/fstat","/getdents","/mkdir_","/openat","/sleep",
                                "/umount","/unlink","/getppid","/waitpid","/yield"};
        
    int status;
    task_count = 0;
    printk("start test\n");
    ECALL(SYS_user_task,run_list[0],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[1],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[2],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[4],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[6],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[9],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[23],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[16],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[7],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[11],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[28],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[8],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[26],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[15],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[17],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[22],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[12],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[25],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[30],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[30],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[29],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[31],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[20],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[19],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[13],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[5],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[21],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[32],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);

    ECALL(SYS_user_task,run_list[10],0,0,0,0,0);
    ECALL(SYS_wait4,-1,&status,0,0,0,0);
        printk("\nend test\n");
    while(1);
    return 1;

}