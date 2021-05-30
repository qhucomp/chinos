#ifndef __SYSCALLS_H
#define __SYSCALLS_H
#include <stdint.h>
#include <sys/types.h>
#include "trap.h"
#define SYS_getcwd          17
#define SYS_dup             23
#define SYS_dup3            24
#define SYS_mkdirat         34
#define SYS_unlinkat        35
#define SYS_linkat          37
#define SYS_umount2         39
#define SYS_mount           40
#define SYS_chdir           49
#define SYS_openat          56
#define SYS_close           57
#define SYS_pipe2           59
#define SYS_getdents64      61
#define SYS_read            63
#define SYS_write           64
#define SYS_fstat           80
#define SYS_exit            93
#define SYS_nanosleep       101
#define SYS_sched_yield     124
#define SYS_times           153
#define SYS_uname           160
#define SYS_gettimeofday    169
#define SYS_getpid          172
#define SYS_getppid         173
#define SYS_brk             214
#define SYS_munmap          215
#define SYS_clone           220
#define SYS_execve          221
#define SYS_mmap            222
#define SYS_wait4           260
#define SYS_start_thread    32
#define SYS_user_task       233
#define AT_FDCWD (-100)
#define SIGCHLD   17
extern char *user_heap_start;
extern char *user_heap_end;

typedef uintptr_t (*syscall_func)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t);
extern syscall_func syscalls[300];
struct utsname {
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
	char domainname[65];
};

struct kstat {
	uint64_t st_dev;
	uint64_t st_ino;
	mode_t st_mode;
	uint32_t st_nlink;
	uint32_t st_uid;
	uint32_t st_gid;
	uint64_t st_rdev;
	unsigned long __pad;
	off_t st_size;
	uint32_t st_blksize;
	int __pad2;
	uint64_t st_blocks;
	long st_atime_sec;
	long st_atime_nsec;
	long st_mtime_sec;
	long st_mtime_nsec;
	long st_ctime_sec;
	long st_ctime_nsec;
};

struct tms              
{                     
	long tms_utime;  
	long tms_stime;  
	long tms_cutime; 
	long tms_cstime; 
};

// struct timespec {
// 	time_t tv_sec;        /* 秒 */
// 	long   tv_nsec;       /* 纳秒, 范围在0~999999999 */
// };

extern struct tms global_tms;
/**
 * @brief 处理系统调用
 * 
 * @param[in] extension 系统调用号
 * 
 * @param[in] reg 寄存器
 * 
 * @return 系统调用的返回
 */
uintptr_t handle_ecall(uintptr_t extension,regs *reg);

ssize_t sys_write(int fd,void *buf,size_t count);
intptr_t sys_brk(size_t pos);
//void sys_exit(int code);
void register_syscall(void);
int sys_wait4(pid_t pid,int *status,int options);
int sys_clone(uintptr_t flags,uintptr_t stack,pid_t ptid,int tls,int ctid);
int sys_exit(int code);
void sys_uname(struct utsname *ptr);
int sys_close(uint64_t fd);	
pid_t sys_getpid(void);
pid_t sys_getppid(void);
int sys_nanosleep(struct timespec *sec);
void *sys_mmap(void *start,size_t len,int prot,int flags,int fd,size_t offset);
int sys_fstat(int fd,struct kstat *stat);
int sys_dup(int fd);
int sys_chdir(char *path);
int sys_pipe(int *fd);
int sys_dup3(int fd,int out);
#endif