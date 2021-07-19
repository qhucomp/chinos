
#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/syscall.h"
#include "include/sysinfo.h"
#include "include/kalloc.h"
#include "include/vm.h"
#include "include/string.h"
#include "include/printf.h"

// Fetch the uint64 at addr from the current process.
int
fetchaddr(uint64 addr, uint64 *ip)
{
  struct proc *p = myproc();
  if(addr >= p->sz || addr+sizeof(uint64) > p->sz)
    return -1;
  // if(copyin(p->pagetable, (char *)ip, addr, sizeof(*ip)) != 0)
  if(copyin2((char *)ip, addr, sizeof(*ip)) != 0)
    return -1;
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Returns length of string, not including nul, or -1 for error.
int
fetchstr(uint64 addr, char *buf, int max)
{
  // struct proc *p = myproc();
  // int err = copyinstr(p->pagetable, buf, addr, max);
  int err = copyinstr2(buf, addr, max);
  if(err < 0)
    return err;
  return strlen(buf);
}

static uint64
argraw(int n)
{
  struct proc *p = myproc();
  switch (n) {
  case 0:
    return p->trapframe->a0;
  case 1:
    return p->trapframe->a1;
  case 2:
    return p->trapframe->a2;
  case 3:
    return p->trapframe->a3;
  case 4:
    return p->trapframe->a4;
  case 5:
    return p->trapframe->a5;
  }
  panic("argraw");
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  *ip = argraw(n);
  return 0;
}

// Retrieve an argument as a pointer.
// Doesn't check for legality, since
// copyin/copyout will do that.
int
argaddr(int n, uint64 *ip)
{
  *ip = argraw(n);
  return 0;
}

// Fetch the nth word-sized system call argument as a null-terminated string.
// Copies into buf, at most max.
// Returns string length if OK (including nul), -1 if error.
int
argstr(int n, char *buf, int max)
{
  uint64 addr;
  if(argaddr(n, &addr) < 0)
    return -1;
  return fetchstr(addr, buf, max);
}

extern uint64 sys_chdir(void);
extern uint64 sys_close(void);
extern uint64 sys_dup(void);
extern uint64 sys_exec(void);
extern uint64 sys_exit(void);
extern uint64 sys_clone(void);
extern uint64 sys_fstat(void);
extern uint64 sys_getpid(void);
extern uint64 sys_kill(void);
extern uint64 sys_tkill(void);
extern uint64 sys_mkdirat(void);
extern uint64 sys_openat(void);
extern uint64 sys_pipe2(void);
extern uint64 sys_read(void);
extern uint64 sys_brk(void);
extern uint64 sys_nanosleep(void);
extern uint64 sys_wait4(void);
extern uint64 sys_write(void);
extern uint64 sys_uptime(void);
// extern uint64 sys_test_proc(void);
extern uint64 sys_dev(void);
// extern uint64 sys_readdir(void);
extern uint64 sys_getcwd(void);
extern uint64 sys_unlinkat(void);
extern uint64 sys_sched_yield(void);
// extern uint64 sys_trace(void);
extern uint64 sys_sysinfo(void);
// extern uint64 sys_rename(void);
// extern uint64 sys_remove(void);
uint64 sys_uname(void);
extern uint64 sys_mmap(void);
extern uint64 sys_munmap(void);
extern uint64 sys_getdents64(void);
extern uint64 sys_dup3(void);
extern uint64 sys_getppid(void);
extern uint64 sys_sched_setaffinity(void);
extern uint64 sys_sched_getaffinity(void);
extern uint64 sys_sched_get_priority_max(void);
extern uint64 sys_sched_get_priority_min(void);
extern uint64 sys_rt_sigaction(void);
extern uint64 sys_rt_sigprocmask(void);
extern uint64 sys_rt_sigreturn(void);
extern uint64 sys_removexattr(void);
extern uint64 sys_setpgid(void);
extern uint64 sys_getpgid(void);
extern uint64 sys_setsid(void);
extern uint64 sys_getsid(void);
extern uint64 sys_getgroups(void);
extern uint64 sys_setgroups(void);
extern uint64 sys_sethostname(void);
extern uint64 sys_getrlimit(void);
extern uint64 sys_setrlimit(void);
extern uint64 sys_umask(void);
extern uint64 sys_getuid(void);
extern uint64 sys_geteuid(void);
extern uint64 sys_getgid(void);
extern uint64 sys_getegid(void);
extern uint64 sys_gettid(void);
extern uint64 sys_fchmod(void);
extern uint64 sys_fchmodat(void);
extern uint64 sys_fchownat(void);
extern uint64 sys_fchown(void);
extern uint64 sys_lsextattr(void);
extern uint64 sys_lseek(void);
extern uint64 sys_readv(void);
extern uint64 sys_writev(void);
extern uint64 sys_sendfile(void);
extern uint64 sys_sync(void);
extern uint64 sys_setuid(void);
extern uint64 sys_setgid(void);
extern uint64 sys_setegid(void);
extern uint64 sys_lsetattr(void);
extern uint64 sys_set_tid_address(void);
// extern 
static uint64 (*syscalls[])(void) = {
  [SYS_clone]                   sys_clone,
  [SYS_exit]                    sys_exit,
  [SYS_wait4]                   sys_wait4,
  [SYS_pipe2]                   sys_pipe2,
  [SYS_read]                    sys_read,
  [SYS_kill]                    sys_kill,
  [SYS_execve]                  sys_exec,
  [SYS_fstat]                   sys_fstat,
  [SYS_chdir]                   sys_chdir,
  [SYS_dup]                     sys_dup,
  [SYS_dup3]                    sys_dup3,
  [SYS_getpid]                  sys_getpid,
  [SYS_getppid]                 sys_getppid,
  [SYS_brk]                     sys_brk,
  [SYS_nanosleep]               sys_nanosleep,
  // [SYS_uptime]      sys_uptime,
  [SYS_openat]                  sys_openat,
  [SYS_write]                   sys_write,
  [SYS_mkdirat]                 sys_mkdirat,
  [SYS_close]                   sys_close,
  // [SYS_test_proc]   sys_test_proc,
  [SYS_dev]                     sys_dev,
  [SYS_getdents64]              sys_getdents64,
  [SYS_getcwd]                  sys_getcwd,
  [SYS_unlinkat]                sys_unlinkat,
  // [SYS_trace]       sys_trace,
  [SYS_uname]                   sys_uname,
  [SYS_sched_yield]             sys_sched_yield,
  [SYS_mmap]                    sys_mmap,
  [SYS_munmap]                  sys_munmap,
  [SYS_tkill]                   sys_tkill,
  [SYS_sysinfo]                 sys_sysinfo,
  [SYS_sched_setaffinity]       sys_sched_setaffinity,
  [SYS_sched_getaffinity]       sys_sched_getaffinity,
  [SYS_sched_get_priority_max]  sys_sched_get_priority_max,
  [SYS_sched_get_priority_min]  sys_sched_get_priority_min,
  [SYS_tkill]                   sys_tkill,
  [SYS_rt_sigaction]            sys_rt_sigaction,
  [SYS_rt_sigprocmask]          sys_rt_sigprocmask,
  [SYS_rt_sigreturn]            sys_rt_sigreturn,
  [SYS_removexattr]             sys_removexattr,
  // [SYS_setpriority]             sys_setpriority,
  [SYS_setgid]                  sys_setgid,
  [SYS_setuid]                  sys_setuid,
  [SYS_getegid]                 sys_getegid,
  [SYS_gettid]                  sys_gettid,
  [SYS_getgid]                  sys_getgid,
  [SYS_geteuid]                 sys_geteuid,
  [SYS_getuid]                  sys_getuid,
  [SYS_fchmod]                  sys_fchmod,
  [SYS_fchmodat]                sys_fchmodat,
  [SYS_fchownat]                sys_fchownat,
  [SYS_fchown]                  sys_fchown,
  // [SYS_lsextattr]               sys_lsextattr,
  [SYS_lseek]                   sys_lseek,
  [SYS_readv]                   sys_readv,
  [SYS_writev]                  sys_writev,
  [SYS_sendfile]                sys_sendfile,
  [SYS_sync]                    sys_sync,
  [SYS_setpgid]                 sys_setpgid,
  [SYS_getpgid]                 sys_getpgid,
  [SYS_setsid]                  sys_setsid,
  [SYS_getgroups]               sys_getgroups,
  [SYS_setgroups]               sys_setgroups,
  [SYS_sethostname]             sys_sethostname,
  [SYS_getrlimit]               sys_getrlimit,
  [SYS_setrlimit]               sys_setrlimit,
  [SYS_umask]                   sys_umask,
  [SYS_lsetxattr]               sys_lsetattr,
  [SYS_set_tid_address]         sys_set_tid_address
  // [SYS_rename]      sys_rename,
};

static char *sysnames[] = {
  [SYS_clone]       "clone",
  [SYS_exit]        "exit",
  [SYS_wait4]       "wait4",
  [SYS_pipe2]       "pipe2",
  [SYS_read]        "read",
  // [SYS_kill]        "kill",
  [SYS_execve]      "execve",
  [SYS_fstat]       "fstat",
  [SYS_chdir]       "chdir",
  [SYS_dup]         "dup",
  [SYS_getpid]      "getpid",
  [SYS_getppid]     "getppid",
  [SYS_brk]         "brk",
  [SYS_nanosleep]   "nanosleep",
  // [SYS_uptime]      "uptime",
  [SYS_openat]      "openat",
  [SYS_write]       "write",
  [SYS_mkdirat]     "mkdirat",
  [SYS_close]       "close",
  // [SYS_test_proc]   "test_proc",
  [SYS_dev]         "dev",
  [SYS_getdents64]  "getdents64",
  [SYS_getcwd]      "getcwd",
  [SYS_unlinkat]    "unlinkat",
  // [SYS_trace]       "trace",
  [SYS_uname]       "uname",
  [SYS_sched_yield] "sched_yield",
  // [SYS_rename]      "rename",
};

void syscall(void)
{
  int num;
  struct proc *p = myproc();
  num = p->trapframe->a7;
    printf("syscall enter %d\n",num);
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    p->trapframe->a0 = syscalls[num]();
        // trace
    if ((p->tmask & (1 << num)) != 0) {
      printf("pid %d: %s -> %d\n", p->pid, sysnames[num], p->trapframe->a0);
    }
  } else {
    printf("pid %d %s: unknown sys call %d\n",
            p->pid, p->name, num);
    p->trapframe->a0 = -1;
  }
}

uint64 sys_uname(void)
{
  uint64 addr;
  // struct proc *p = myproc();

  if (argaddr(0, &addr) < 0) {
    return -1;
  }

  struct utsname info;
  strncpy(info.sysname,"chino os",8);
  strncpy(info.nodename,"chino",5);
  strncpy(info.release,"0",1);
  strncpy(info.version,"0.02",4);
  strncpy(info.machine,"riscv64",7);
  strncpy(info.domainname,"chino",5);
  if (copyout2(addr, (char *)&info, sizeof(info)) < 0)
    return -1;

  return 0;
}