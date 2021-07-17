
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
static int
loadseg(pagetable_t pagetable, uint64 va, struct dirent *ep, uint offset, uint sz)
{
  uint i, n;
  uint64 pa;
  // int first = 1;
  // uint off = 0;
  if((va % PGSIZE) != 0)
    panic("loadseg: va must be page aligned");
  printf("enter loadseg\n");
  for(i = 0; i < sz; i += PGSIZE){
    pa = walkaddr(pagetable, va + i);
    if(pa == (uint64)NULL)
      panic("loadseg: address should exist");
    if(sz - i < PGSIZE)
      n = sz - i;
    else
      n = PGSIZE;
    // printf("%p\n",va+i+va_offset);
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
}


int exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint64 argc, sz = 0, sp, ustack[MAXARG+1], stackbase;
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
  uint va_offset;
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    va_offset = 0;
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
      printf("bad 4\n");
      printf("vaddr:%p\n",ph.vaddr);
      uint64 oldaddr = ph.vaddr;
      ph.vaddr = PGROUNDDOWN(ph.vaddr);
      va_offset = oldaddr - ph.vaddr;
      ph.off = PGROUNDDOWN(ph.off);
      // ph.filesz = 13696;
      ph.memsz += va_offset;
      printf("filesz:%d offset:%d va_offset:%d\n",ph.filesz,ph.off,va_offset);
      // goto bad;
      // continue;
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
  if((sz1 = uvmalloc(pagetable, kpagetable, sz, sz + 2*PGSIZE)) == 0)
    goto bad;
  sz = sz1;
  uvmclear(pagetable, sz-2*PGSIZE);
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
  ustack[argc] = 0;

  // push the array of argv[] pointers.
  sp -= (argc+1) * sizeof(uint64);
  sp -= sp % 16;
  if(sp < stackbase)
    goto bad;
  if(copyout(pagetable, sp, (char *)ustack, (argc+1)*sizeof(uint64)) < 0)
    goto bad;

  // arguments to user main(argc, argv)
  // argc is returned via the system call return
  // value, which goes in a0.
  p->trapframe->a1 = sp;

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
