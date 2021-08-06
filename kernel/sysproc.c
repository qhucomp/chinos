
#include "include/types.h"
#include "include/riscv.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/syscall.h"
#include "include/timer.h"
#include "include/kalloc.h"
#include "include/string.h"
#include "include/printf.h"
#include "include/param.h"
#include "include/vm.h"
extern int exec(char *path, char **argv);
extern struct proc proc[NPROC];
uint64 sys_exec(void)
{
  char path[FAT32_MAX_PATH], *argv[MAXARG];
  int i;
  uint64 uargv, uarg;

  if(argstr(0, path, FAT32_MAX_PATH) < 0 || argaddr(1, &uargv) < 0){
    printf("return -1\n");
    return -1;
  }
  memset(argv, 0, sizeof(argv));
  for(i=0;; i++){
    if(i >= NELEM(argv)){
      goto bad;
    }
    if(fetchaddr(uargv+sizeof(uint64)*i, (uint64*)&uarg) < 0){
      goto bad;
    }
    if(uarg == 0){
      argv[i] = 0;
      break;
    }
    argv[i] = kalloc();
    if(argv[i] == 0)
      goto bad;
    if(fetchstr(uarg, argv[i], PGSIZE) < 0)
      goto bad;
  }

  int ret = exec(path, argv);
  printf("execve ret=%d\n",ret);
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);

  return ret;

 bad:
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);
  printf("execve failed\n");
  return -1;
}

uint64 sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64 sys_getpid(void)
{
  return myproc()->pid;
}

uint64 sys_getppid(void) {
  return myproc()->parent->pid;
}

uint64 sys_clone(void)
{
  return fork();
}

uint64 sys_wait4(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64 sys_brk(void)
{
  int addr;
  int n;
  if(argint(0, &n) < 0)
    return -1;

  if (n)
    addr = n;
  else
    addr = myproc()->sz;

  if(growproc(n) < 0)
    return -1;
  printf("n:%p ret:%p\n",n,addr);
  return addr;
}

uint64 sys_nanosleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64 sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}
uint64 sys_tkill(void) {
  return sys_kill();
}

// return how many clock tick interrupts have occurred
// since start.
uint64 sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_trace(void)
{
  int mask;
  if(argint(0, &mask) < 0) {
    return -1;
  }
  myproc()->tmask = mask;
  return 0;
}

uint64 sys_sched_yield(void) {
  yield();
  return 0;
}

uint64 sys_mmap(void) {
  uint64 start;
  int len,fd,offset;
  if(argaddr(0,&start) < 0 || argint(1,&len) < 0 || argint(4,&fd) < 0 || argint(5,&offset) < 0)
    return -1;
  struct proc *proc = myproc();
  int error = -1;;
  for(int i = 0;i < NOFILE;i++) {
    if (proc->mmap_address[i] == 0) {
      error = start;
      proc->mmap_address[i] = (char *)start;
      proc->len[i] = len;
      proc->offset[i] = offset;
      proc->fd[i] = fd;
    }
  }
  elock(proc->ofile[fd]->ep);
  eread(proc->ofile[fd]->ep,1,start,offset,len);
  eunlock(proc->ofile[fd]->ep);
  return error;
}

uint64 sys_munmap(void) {
  uint64 start;
  int len,fd,offset;
  if(argaddr(0,&start) < 0 || argint(1,&len) < 0)
    return -1;
  struct proc *proc = myproc();
  int error = -1;
  for(int i = 0;i < NOFILE;i++) {
    if(proc->mmap_address[i] == (char *)start) {
      proc->mmap_address[i] = NULL;
      fd = proc->fd[i];
      offset = proc->offset[i];
      error = 0;
      break;
    }
  }
  if(!error)
    ewrite(proc->ofile[fd]->ep,1,start,offset,len);
  return error;

}

extern int argfd(int n, int *pfd, struct file **pf);

uint64 sys_sched_setaffinity(void) {
  int pid,cpusetsize;
  uint64 mask;
  if(argint(0,&pid) < 0 || argint(1,&cpusetsize) < 0 || argaddr(2,&mask) < 0)
    return -1;
  if((pid == 0) || (pid == myproc()->pid)){  //检查权限
    if(copyin2((char *)&proc[pid].cpu_mask,mask,sizeof(cpu_set_t)) < 0)
      return -1;
  } else
    return -1;
  return 0;
}

uint64 sys_sched_getaffinity(void) {
  int pid,cpusetsize;
  uint64 mask;
  if(argint(0,&pid) < 0 || argint(1,&cpusetsize) < 0 || argaddr(2,&mask) < 0)
    return -1;
  if(pid == 0 || pid == myproc()->pid){  //检查权限
    if(copyout2(mask,(char *)&proc[pid].cpu_mask,sizeof(cpu_set_t)) < 0)
      return -1;
  } else
    return -1;
  return 0;
}

uint64 sys_sched_get_priority_max(void) {
  return myproc()->max_nice;
}
uint64 sys_sched_get_priority_min(void) {
  return myproc()->min_nice;
}
#define SIGHUP  1
#define SIGINT  2
#define SIGQUIT 3
#define SIGILL  4
#define SIGABRT 6
#define SIGFPE  8
#define SIGKILL 9
#define SIGSEGV 11
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15

//TODO
uint64 sys_rt_sigaction(void) {
  return 0;
}

uint64 sys_rt_sigprocmask(void) {
  return 0;
}

uint64 sys_rt_sigreturn(void) {
  return 0;
}

uint64 sys_removexattr(void) {
  return 0;
}

uint64 sys_setpgid(void) {
  int pgid;
  int pid;
  if(argint(0,&pid) < 0 || argint(1,&pgid) < 0)
    return -1;
  
  if (proc[pid].state == RUNNABLE) {
    proc[pid].pgid = pgid;
    return 0;
  } else {
    return -1;
  }
}

uint64 sys_getpgid(void) {
  int pid;
  if(argint(0,&pid) < 0)
    return -1;
  if(proc[pid].state == RUNNABLE)
    return proc[pid].pgid;
  else
    return -1;
}

uint64 sys_setsid(void) {
  int sid;
  int pid;
  if(argint(0,&pid) < 0 || argint(1,&sid) < 0)
    return -1;
  
  if (proc[pid].state == RUNNABLE) {
    proc[pid].sid = sid;
    return 0;
  } else {
    return -1;
  }
}

uint64 sys_getsid(void) {
  int pid;
  if(argint(0,&pid) < 0) 
    return -1;
  if(proc[pid].state == RUNNABLE)
    return proc[pid].sid;
  else
    return -1;
}

uint64 sys_getgroups(void) {
  int size;
  uint64 list;
  gid_t *glist;
  if(argint(0,&size) < 0 || argaddr(1,&list) < 0)
    return -1;

  glist = (gid_t *)list;
  if(!size)
    return myproc()->list_len;
  else {
    for(int i = 0;i < size;i++)
      glist[0] = myproc()->gid_list[i];
  }
  return size;
}

uint64 sys_setgroups(void) {
  int size;
  gid_t * glist;
  uint64 list;
  if(argint(0,&size) < 0|| argaddr(1,&list) < 0)
    return -1;
  if(size > 32)
    return -1;
  glist = (gid_t *)list;
  for(int i = 0;i < size;i++)
    myproc()->gid_list[i] = glist[i];
  return 0;
}

char hostname[256] = "chino";
uint64 sys_sethostname(void) {
  int len;
  uint64 host;
  if(argint(0,&len) < 0 || argaddr(1,&host) < 0)
    return -1;
  if(copyin2(hostname,host,len) < 0)
    return -1;
  return 0;
}

uint64 sys_getrlimit(void) {
  return 0;
}

uint64 sys_setrlimit(void) {
  return 0;
}

uint64 sys_umask(void) {
  int mask;
  if(argint(0,&mask) < 0)
    return -1;
  
  myproc()->mask = mask;
  return 0;
}

uint64 sys_setuid(void) {
  int uid;
  if(argint(0,&uid) < 0)
    return -1;
  if(myproc()->uid == 0)
    myproc()->uid = uid;
  return 0;
}

uint64 sys_setgid(void) {
  int gid;
  if(argint(0,&gid) < 0)
    return -1;
  if(myproc()->uid == 0)
    myproc()->gid = gid;
  return 0;
}

uint64 sys_getuid(void) {
  return 0;//myproc()->uid;
}
uint64 sys_geteuid(void) {
  return 0;//myproc()->euid;
}
uint64 sys_getgid(void) {
  return 0;//myproc()->gid;
}
uint64 sys_getegid(void) {
  return 0;//myproc()->egid;
}
uint64 sys_gettid(void) {
  return myproc()->tid;
}

struct sysinfo {
    long uptime;             /* Seconds since boot */
    unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
    unsigned long totalram;  /* Total usable main memory size */
    unsigned long freeram;   /* Available memory size */
    unsigned long sharedram; /* Amount of shared memory */
    unsigned long bufferram; /* Memory used by buffers */
    unsigned long totalswap; /* Total swap space size */
    unsigned long freeswap;  /* Swap space still available */
    unsigned short procs;    /* Number of current processes */
    unsigned long totalhigh; /* Total high memory size */
    unsigned long freehigh;  /* Available high memory size */
    unsigned int mem_unit;   /* Memory unit size in bytes */
    char _f[20-2*sizeof(long)-sizeof(int)];
                            /* Padding to 64 bytes */
};
extern uint64 totalram;
extern uint64 freeram;
uint64 sys_sysinfo(void) {
  struct sysinfo p;
  uint64 addr;
  if(argaddr(0,&addr) < 0)
    return -1;
  p.uptime = sys_uptime();
  p.loads[0] = p.loads[1] = p.loads[2] = 0;
  p.totalram = totalram;
  p.freeram = freeram;
  p.sharedram = 0;
  p.bufferram = 0;
  p.freeswap = 0;
  p.procs = 0;
  for(int i = 0;i < NPROC;i++)
    if(proc[i].state == RUNNABLE)
      p.procs++;
  p.totalhigh = totalram;
  p.freehigh = freeram;
  p.mem_unit = 0;
  if(copyout2(addr,(char *)&p,sizeof(p)) < 0)
    return -1;
  return 0;
}

//TODO
uint64 sys_fchmod(void) {
  return 0;
}
uint64 sys_fchmodat(void) {
  return 0;
}
uint64 sys_fchownat(void) {
  return 0;
}
uint64 sys_fchown(void) {
  return 0;
}
uint64 sys_lsetattr(void) {
  return 0;
}

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
uint64 sys_lseek(void) {
  int fd,offset,whence;
  if(argint(0,&fd) < 0 || argint(1,&offset) < 0 || argint(2,&whence) < 0)
    return -1;
  switch(whence) {
    case SEEK_SET:
      myproc()->ofile[fd]->off = offset;
      break;
    case SEEK_CUR:
      myproc()->ofile[fd]->off += offset;
      break;
    case SEEK_END:
      elock(myproc()->ofile[fd]->ep);
      myproc()->ofile[fd]->off = myproc()->ofile[fd]->ep->file_size + offset;
      eunlock(myproc()->ofile[fd]->ep);
      break;
  }
  return myproc()->ofile[fd]->off; 
}

struct iovec {
    void  *iov_base;    /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
};
uint64 sys_readv(void) {
  int iovcnt;
  uint64 iov;
  struct file *fp;
  if(argfd(0,NULL,&fp) < 0 || argaddr(1,&iov) < 0 || argint(2,&iovcnt) < 0)
    return -1;
  
  struct iovec *vec = (struct iovec *)iov;
  uint64 size = 0;
  for(int i = 0;i < iovcnt;i++) {
    uint64 s = fileread(fp,(uint64)vec->iov_base,vec[i].iov_len);
    if(s != -1)
      size += s;
    else
      break;
  }
  return size;
}

uint64 sys_writev(void) {
  int iovcnt;
  uint64 iov;
  struct file *fp;
  if(argfd(0,NULL,&fp) < 0 || argaddr(1,&iov) < 0 || argint(2,&iovcnt) < 0)
    return -1;
  
  struct iovec *vec = (struct iovec *)iov;
  uint64 size = 0;
  for(int i = 0;i < iovcnt;i++) {
    printf("iov_len:%d\n",vec[i].iov_len);
    uint64 s = filewrite(fp,(uint64)vec->iov_base,vec[i].iov_len);
    if(s != -1)
      size += s;
    else
      break;
  }
  return size;
}

uint64 sys_sendfile(void) {
  struct file *out_fd,*in_fd;
  uint64 offset;
  uint64 count;
  if(argfd(0,NULL,&out_fd) < 0 || argfd(1,NULL,&in_fd) < 0 || argaddr(2,&offset) < 0 || argaddr(3,&count) < 0)
    return -1;
  
  char buf[2048];
  size_t total_size = 0;
  while(count < 0) {
    int ret = fileread(in_fd,(uint64)buf,2048);
    count -= ret;
    total_size += ret;
    filewrite(out_fd,(uint64)buf,ret);
    if(ret != 2048)
      break;
  }
  if(offset != 0)
    copyout2(offset,(char *)&in_fd->off,sizeof(uint));
  return total_size;
}
uint64 sys_sync(void) {
  return 0;
}
uint64 sys_utimensat(void) {
  return 0;
}

uint64 sys_set_tid_address(void) {
  uint64 tid;
  if(argaddr(0,&tid) < 0)
    return -1;
  copyout2(tid,(char *)&myproc()->tid,sizeof(int));
  return myproc()->tid;
}

uint64 sys_faccessat(void) {
  char path[1024];
  argstr(1, path, FAT32_MAX_PATH);
  printf("path:%s\n",path);
  return -1;
}	

uint64 sys_mprotect(void) {
  return 0;
}

uint64_t sys_readlinkat(void) {
  uint64 buf;
  if(argaddr(2,&buf) < 0)
    return -1;
  char path[13] = "/lmbench_all";
  copyout2(buf,path,sizeof(path));
  return 5;
}

uint64 sys_ioctl(void) {
  return 0;
}