
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

extern int exec(char *path, char **argv);

uint64 sys_exec(void)
{
  char path[FAT32_MAX_PATH], *argv[MAXARG];
  int i;
  uint64 uargv, uarg;

  if(argstr(0, path, FAT32_MAX_PATH) < 0 || argaddr(1, &uargv) < 0){
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
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);

  return ret;

 bad:
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);
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
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
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

// uint64 sys_kill(void)
// {
//   int pid;

//   if(argint(0, &pid) < 0)
//     return -1;
//   return kill(pid);
// }

// return how many clock tick interrupts have occurred
// since start.
// uint64 sys_uptime(void)
// {
//   uint xticks;

//   acquire(&tickslock);
//   xticks = ticks;
//   release(&tickslock);
//   return xticks;
// }

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
  sched();
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
      error = 0;
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
  // ewrite(f->ep, 1, addr, f->off, n);
}