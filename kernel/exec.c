
#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/spinlock.h"
#include "include/sleeplock.h"
#include "include/proc.h"
#include "include/elf.h"
#include "include/fat32.h"
#include "include/kalloc.h"
#include "include/vm.h"
#include "include/printf.h"
#include "include/string.h"

// Load a program segment into pagetable at virtual address va.
// va must be page-aligned
// and the pages from va to va+sz must already be mapped.
// Returns 0 on success, -1 on failure.
#define min(a,b) (a > b ? b : a)
static int
loadseg(pagetable_t pagetable, uint64 va, struct dirent *ep, uint offset, uint sz)
{
  uint i, n;
  uint64 pa;
  // int first = 1;
  // uint off = 0;
  // if((va % PGSIZE) != 0)
  //   panic("loadseg: va must be page aligned");
  printf("enter loadseg\n");
  for(i = 0; i < sz; i += PGSIZE){
    pa = walkaddr(pagetable, va + i);
    if(pa == (uint64)NULL)
      panic("loadseg: address should exist");
    if(sz - i < PGSIZE)
      n = sz - i;
    else
      n = PGSIZE;
    printf("%p\n",va+i);
    if(eread(ep, 0, (uint64)pa, offset+i, n) != (n)) {
      printf("load error\n");
      return -1;
    }
    // if(first)
    //   off = va_offset;
    // first = 0;
    // va_offset = 0;
  }
  printf("load successfully\n");
  return 0;
  // uint   i, n;
  // uint64 pa;

  // uint32_t newsz = sz > 10 * PGSIZE ? 10 * PGSIZE : sz;

  // for (i = 0; i < newsz; i += n) {
  //   pa = walkaddr(pagetable, va + i);
  //   if (pa == 0)
  //     panic("loadseg: address should exist");
  //   pa += (va + i) % PGSIZE;
  //   n = min(newsz - i, PGSIZE - pa % PGSIZE);
  //   printf("va=%p pa=%p n=%d\n", va, pa, n);
  //   if (eread(ep, pa, 0, offset + i, n) != n) {
  //     return -1;
  //   }
  // }
  return 0;
}

char *env[] ={"SHELL=shell",
              "PWD=/",
              "HOME=/",
              "USER=root",
              "MOTD_SHOWN=pam",
              "LANG=C.UTF-8",
              "INVOCATION_ID=e9500a871cf044d9886a157f53826684",
              "TERM=vt220",
              "SHLVL=2",
              "JOURNAL_STREAM=8:9265",
              "PATH=/",
              "OLDPWD=/root",
              "_=busybox",
              0};
static uint64 CopyString(const char *s,pagetable_t pg,uint64 *sp)
{
  *sp -= strlen(s) + 1;
  *sp -= *sp % 16;
  if (copyout(pg, *sp, (char *)s, strlen(s) + 1) < 0)
    return -1;
  return *sp;
}

uint64_t CreateUserStack(uint64 *ustack, struct elfhdr *elf,int argc,int envc,pagetable_t pg,uint64 *sp)
{
  int index = argc + envc + 2;

  uint64_t filename = CopyString("./lua",pg,sp);
#define NEW_AUX_ENT(id, val)                                      \
  do {                                                            \
    ustack[index++] = id;                                         \
    ustack[index++] = val;                                        \
  } while (0)

  // 1
  // 2
  NEW_AUX_ENT(0x28, 0);
  NEW_AUX_ENT(0x29, 0);
  NEW_AUX_ENT(0x2a, 0);
  NEW_AUX_ENT(0x2b, 0);
  NEW_AUX_ENT(0x2c, 0);
  NEW_AUX_ENT(0x2d, 0);
#define AT_NULL 0      /* end of vector */
#define AT_IGNORE 1    /* entry should be ignored */
#define AT_EXECFD 2    /* file descriptor of program */
#define AT_PHDR 3      /* program headers for program */
#define AT_PHENT 4     /* size of program header entry */
#define AT_PHNUM 5     /* number of program headers */
#define AT_PAGESZ 6    /* system page size */
#define AT_BASE 7      /* base address of interpreter */
#define AT_FLAGS 8     /* flags */
#define AT_ENTRY 9     /* entry point of program */
#define AT_NOTELF 10   /* program is not ELF */
#define AT_UID 11      /* real uid */
#define AT_EUID 12     /* effective uid */
#define AT_GID 13      /* real gid */
#define AT_EGID 14     /* effective gid */
#define AT_PLATFORM 15 /* string identifying CPU for optimizations */
#define AT_HWCAP 16    /* arch dependent hints at CPU capabilities */
#define AT_CLKTCK 17   /* frequency at which times() increments */
/* AT_* values 18 through 22 are reserved */
#define AT_SECURE 23 /* secure mode boolean */
#define AT_BASE_PLATFORM                                                       \
  24 /* string identifying real platform, may differ from AT_PLATFORM. */
#define AT_RANDOM 25 /* address of 16 random bytes */

#define AT_EXECFN 31 /* filename of program */

#define AT_VECTOR_SIZE_BASE 19 /* NEW_AUX_ENT entries in auxiliary table */

  NEW_AUX_ENT(AT_PHDR, elf->phoff);               // 3
  NEW_AUX_ENT(AT_PHENT, sizeof(struct proghdr));  // 4
  NEW_AUX_ENT(AT_PHNUM, elf->phnum);              // 5
  NEW_AUX_ENT(AT_PAGESZ, 0x1000);                 // 6
  NEW_AUX_ENT(AT_BASE, 0);                        // 7
  NEW_AUX_ENT(AT_FLAGS, 0);                       // 8
  NEW_AUX_ENT(AT_ENTRY, elf->entry);              // 9
  NEW_AUX_ENT(AT_UID, 0);                         // 11
  NEW_AUX_ENT(AT_EUID, 0);                        // 12
  NEW_AUX_ENT(AT_GID, 0);                         // 13
  NEW_AUX_ENT(AT_EGID, 0);                        // 14
  NEW_AUX_ENT(AT_HWCAP, 0x112d);                  // 16
  NEW_AUX_ENT(AT_CLKTCK, 64);                     // 17
  NEW_AUX_ENT(AT_EXECFN, filename);               // 31
  NEW_AUX_ENT(0, 0);

#undef NEW_AUX_ENT
  *sp -= sizeof(uint64_t) * index;
  if (copyout(pg, *sp,(char *)ustack, sizeof(uint64_t) * index)) {
    return -1;
  }
  uint64_t argc2 = argc;
  *sp -= sizeof(uint64_t);
  copyout(pg, *sp, (char *)&argc2,
          sizeof(uint64_t));
  return 0;
}

int exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint64 argc, sz = 0, sp, ustack[MAXARG+1], stackbase;
  // uint64 envk[16];
  struct elfhdr elf;
  struct dirent *ep;
  struct proghdr ph;
  pagetable_t pagetable = 0, oldpagetable;
  pagetable_t kpagetable = 0, oldkpagetable;
  struct proc *p = myproc();

  // Make a copy of p->kpt without old user space, 
  // but with the same kstack we are using now, which can't be changed
  if ((kpagetable = (pagetable_t)kalloc()) == NULL) {
    return -1;
  }
  memmove(kpagetable, p->kpagetable, PGSIZE);
  for (int i = 0; i < PX(2, MAXUVA); i++) {
    kpagetable[i] = 0;
  }
  if((ep = ename(path)) == NULL) {
    #ifdef DEBUG
    printf("[exec] %s not found\n", path);
    #endif
    goto bad;
  }
  printf("open file OK\n");
  elock(ep);
  // Check ELF header
  if(eread(ep, 0, (uint64) &elf, 0, sizeof(elf)) != sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;
  if((pagetable = proc_pagetable(p)) == NULL)
    goto bad;
  printf("Check ELF header OK\n");
  // Load program into memory.
  // uint va_offset;
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    // va_offset = 0;
    if(eread(ep, 0, (uint64)&ph, off, sizeof(ph)) != sizeof(ph)) {
      printf("bad 1\n");
      goto bad;
    }
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz) {
      printf("bad 2\n");
      goto bad;
    }
    if(ph.vaddr + ph.memsz < ph.vaddr) {
      printf("bad 3\n");
      goto bad;
    }
    uint64 sz1;
    if(ph.vaddr % PGSIZE != 0) {
      printf("need align pgsize\n");
    }
    if((sz1 = uvmalloc(pagetable, kpagetable, sz, ph.vaddr + ph.memsz + 0x4000)) == 0) {
      printf("bad 3\n");
      goto bad;
    }
    sz = sz1;

    if(loadseg(pagetable, ph.vaddr, ep, ph.off, ph.filesz) < 0) {
      printf("bad 5\n");
      goto bad;
    }
  }
  printf("Load program into memory OK\n");
  eunlock(ep);
  eput(ep);
  ep = 0;

  p = myproc();
  uint64 oldsz = p->sz;

  // Allocate two pages at the next page boundary.
  // Use the second as the user stack.
  sz = PGROUNDUP(sz);
  uint64 sz1;
  if((sz1 = uvmalloc(pagetable, kpagetable, sz, sz + PGSIZE)) == 0)
    goto bad;
  sz = sz1;
  // uvmclear(pagetable, sz-2*PGSIZE);
  sp = sz;
  stackbase = sp - PGSIZE;

  // Push argument strings, prepare rest of stack in ustack.
  for(argc = 0; argv[argc]; argc++) {
    if(argc >= MAXARG)
      goto bad;
    sp -= strlen(argv[argc]) + 1;
    sp -= sp % 16; // riscv sp must be 16-byte aligned
    if(sp < stackbase)
      goto bad;
    if(copyout(pagetable, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
      goto bad;
    ustack[argc] = sp;
  }

  int envc;
  for(envc = 0; env[envc]; envc++) {
    if(envc + argc >= MAXARG)
      goto bad;
    sp -= strlen(env[envc]) + 1;
    sp -= sp % 16; // riscv sp must be 16-byte aligned
    if(sp < stackbase)
      goto bad;
    if(copyout(pagetable, sp, env[envc], strlen(env[envc]) + 1) < 0)
      goto bad;
    ustack[envc + argc] = sp;
  }

  ustack[envc + argc] = 0;

  // push the array of argv[] pointers.
  sp -= (argc+envc+1) * sizeof(uint64);
  sp -= sp % 16;
  if(sp < stackbase)
    goto bad;
  // if(copyout(pagetable, sp, (char *)ustack, (argc+envc+1)*sizeof(uint64)) < 0)
  //   goto bad;
  CreateUserStack(ustack,&elf,argc,envc,pagetable,&sp);
  // arguments to user main(argc, argv)
  // argc is returned via the system call return
  // value, which goes in a0.
  // p->trapframe->a2 = sp;

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(p->name, last, sizeof(p->name));
    
  // Commit to the user image.
  oldpagetable = p->pagetable;
  oldkpagetable = p->kpagetable;
  p->pagetable = pagetable;
  p->kpagetable = kpagetable;
  p->sz = sz;
  p->trapframe->epc = elf.entry;  // initial program counter = main
  printf("epc:%p\n",p->trapframe->epc);
  p->trapframe->sp = sp; // initial stack pointer
  proc_freepagetable(oldpagetable, oldsz);
  w_satp(MAKE_SATP(p->kpagetable));
  sfence_vma();
  kvmfree(oldkpagetable, 0);
  return argc; // this ends up in a0, the first argument to main(argc, argv)
  // sz = PGROUNDUP(sz);
  // uint64_t sz1;
  // if ((sz1 = uvmalloc(pagetable, sz, sz + PGSIZE)) == 0)
  //   goto bad;
  // sz = sz1;
  // sp = sz;
  // stackbase = sz - PGSIZE;

  // sp -= sizeof(uint64_t);
  // a0 = sp;

  // // 先将参数push到用户栈中，并准备ustack数组，它的每一个
  // // 元素都按顺序指向参数。

  // bin_prog.stack_top = 0;
  // bin_prog.sp = sp;
  // bin_prog.stackbase = stackbase;
  // bin_prog.pagetable = pagetable;
  // bin_prog.ustack = ustack;

  // bin_prog.argc = bin_prog.CopyString2Stack(argv);
  // bin_prog.envc = bin_prog.CopyString2Stack((char **)env);
  // CreateUserStack(&bin_prog, &elf);

  // sp = bin_prog.sp;

  // // 用户代码main(argc, argv)的参数
  // // argc通过系统调用返回，也就是a0
  // p->trapframe->a1 = sp;
  // // 保存程序名
  // char *last, *s;
  // for (last = s = path; *s; s++)
  //   if (*s == '/')
  //     last = s + 1;
  // safestrcpy(p->name, last, sizeof(p->name) + 1);

  // p->trapframe->ra = 0;
  // oldpagetable = p->pagetable;
  // p->pagetable = pagetable;
  // p->sz = sz;
  // p->trapframe->epc = elf.entry;
  // p->trapframe->sp = sp;
  // printf("sp=%p oldsz=%d sz=%d entry=%p", sp, oldsz, sz, elf.entry);
  // kvmfree(oldpagetable, oldsz);
  // ip->free();
  // task->lock.lock();
  // task->lock.unlock();
  return 0;
 bad:
  #ifdef DEBUG
  printf("[exec] reach bad\n");
  #endif
  if(pagetable)
    proc_freepagetable(pagetable, sz);
  if(kpagetable)
    kvmfree(kpagetable, 0);
  if(ep){
    eunlock(ep);
    eput(ep);
  }
  return -1;
}
