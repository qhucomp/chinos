
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
   //if((va % PGSIZE) != 0)
   //  va = PGROUNDDOWN(va);
  //   panic("loadseg: va must be page aligned");
  uint64 off = va - PGROUNDDOWN(va);
  printf("off:%d",off);
  printf("enter loadseg\n");
  for(i = 0; i < sz; i += PGSIZE){
    pa = walkaddr(pagetable, va + i);
    if(pa == (uint64)NULL)
      panic("loadseg: address should exist");
    if(sz - i < PGSIZE)
      n = sz - i;
    else
      n = PGSIZE;
    printf("va:%p pa:%p n:%d\n",va+i,pa,n);
    if(eread(ep, 0, (uint64)pa + off, offset+i, n) != (n)) {
      printf("load error\n");
      return -1;
    }
  }
  printf("load successfully\n");
  return 0;
}

char *env[] ={"SHELL=shell",
              "PWD=/",
              "HOME=/",
              "USER=root",
              "MOTD_SHOWN=pam",
              "LANG=C.UTF-8",
              "INVOCATION_ID=4a5537a1084a45c49a9645327fdf3922",
              "TERM=vt220",
              "SHLVL=1",
              "JOURNAL_STREAM=8:9265",
              "PATH=/",
              "OLDPWD=/",
              "HUSHLOGIN=FALSE",
              "_=/busybox",
              0};
static uint64 CopyString(const char *s,pagetable_t pg,uint64 *sp)
{
  *sp -= strlen(s) + 1;
  *sp -= *sp % 16;
  if (copyout(pg, *sp, (char *)s, strlen(s) + 1) < 0)
    return -1;
  return *sp;
}

/*uint64_t CreateUserStack(uint64 *ustack, struct elfhdr *elf,int argc,int envc,pagetable_t pg,uint64 *sp,uint64 load_addr)
{
  int index = argc + envc + 2;

  uint64_t filename = CopyString("./a.out ",pg,sp);
#define NEW_AUX_ENT(id, val)                                      \
  do {                                                            \
    ustack[index++] = id;                                         \
    ustack[index++] = val;                                        \
  } while (0)

  uint64 random = 0x32123;
  NEW_AUX_ENT(AT_HWCAP, 0);                       // 16
  NEW_AUX_ENT(AT_PAGESZ, 0x1000);                 // 6
  NEW_AUX_ENT(AT_CLKTCK, 64);                     // 17
  NEW_AUX_ENT(AT_PHDR, elf->phoff + load_addr);   // 3
  NEW_AUX_ENT(AT_PHENT, sizeof(struct proghdr));  // 4
  NEW_AUX_ENT(AT_PHNUM, elf->phnum);              // 5
  NEW_AUX_ENT(AT_BASE, 0);                        // 7
  NEW_AUX_ENT(AT_FLAGS, 0);                       // 8
  NEW_AUX_ENT(AT_ENTRY, elf->entry);              // 9
  NEW_AUX_ENT(AT_UID, 0);                         // 11
  NEW_AUX_ENT(AT_EUID, 0);                        // 12
  NEW_AUX_ENT(AT_GID, 0);                         // 13
  NEW_AUX_ENT(AT_EGID, 0);                        // 14
  NEW_AUX_ENT(AT_SECURE,0);
  NEW_AUX_ENT(AT_RANDOM,random);
  NEW_AUX_ENT(AT_EXECFN, filename);               // 31
  //NEW_AUX_ENT(AT_PLATFORM, 0);

  //NEW_AUX_ENT(AT_BASE_PLATFORM, 0);
  //NEW_AUX_ENT(AT_EXECFD, 3);
  NEW_AUX_ENT(0, 0);

#undef NEW_AUX_ENT
  *sp -= sizeof(uint64_t) * index;
  if (copyout(pg, *sp,(char *)ustack, sizeof(uint64_t) * index)) {
    return -1;
  }
  uint64_t argc2 = argc;
  *sp -= sizeof(uint64_t);
  copyout(pg, *sp, (char *)&argc2,sizeof(uint64_t));
  return 0;
}*/
typedef struct bin_program {
    uint64_t    sp;
    uint64_t    stackbase;
    void* pagetable;
    char *      filename;
    int         argc, envc;
    uint64_t *  ustack;  // 用户栈(低->高): [argc|argv|env|elf_info]
    int         stack_top;
} bin_program_t;
void bin_program_init(bin_program_t *bin)
{
    bin->sp = 0;
    bin->stackbase = 0;
    bin->pagetable = NULL;
    bin->filename = NULL;
    bin->argc = 0;
    bin->envc = 0;
    bin->ustack = NULL;
    bin->stack_top = 0;
}
static int merge_args(char *args[], char *argv[], char *envp[])
{
    int i, j = 0;
    for (i = 0; i < MAXARG; i++) {
        if (argv[i] != NULL) {
#ifdef DEBUG_ARGS
            print("[proc] argv[%d]=%s\n", i, argv[i]);
#endif
            args[j++] = argv[i];
        } else {
            if (i == 0) {   /* 没有任何参数，不过还是需要至少保留一个参数 */
                printf("[proc] no argv at %d\n", j);
                args[j++] = NULL;
            }
            break;
        }
    }
#ifdef DEBUG_ARGS
    print("[proc] argv end at %d\n", j);
#endif
    args[j++] = NULL; // 中间预留一个0，因为环境和参数中间要间隔一个0
    for (i = 0; i < MAXARG; i++) {
        if (envp[i] != NULL) {
#ifdef DEBUG_ARGS
            dbgprint("[proc] envp[%d]=%p\n", i, envp[i]);
#endif
            args[j++] = envp[i];

        } else {
            if (i == 0) {   /* 没有任何参数，不过还是需要至少保留一个参数 */
                printf("[proc] no envp at %d\n", j);
                args[j++] = NULL;
            }
            break;
        }
    }
    args[j++] = NULL;
    return j;
}
int bin_program_copy_string2stack(bin_program_t *bin, char *strs[])
{
    int i = bin->stack_top;
    for (; strs[i]; i++) {
        if (i > MAXARG)
            return -1;
        bin->sp -= strlen(strs[i]) + 1;
        bin->sp -= bin->sp % 16;
        if (bin->sp < bin->stackbase) {
            return -1;
        }
        if (copyout(bin->pagetable, bin->sp, strs[i], strlen(strs[i]) + 1) < 0)
            return -1;
        bin->ustack[i] = bin->sp;
    }
    bin->ustack[i] = 0;
    int c = i - bin->stack_top;
    bin->stack_top = i + 1;
    return c;
}
uint64_t bin_program_copy_nbytes(bin_program_t *bin, char *buf, size_t len)
{
    bin->sp -= len + 1;
    bin->sp -= bin->sp % 16;
    if (copyout(bin->pagetable, bin->sp, (char *)buf, len + 1) < 0)
        return -1;
    return bin->sp;
}

static uint64_t create_user_stack(bin_program_t *bin_program,struct elfhdr *elf,struct proghdr *elf_phdr)
{
    int index = bin_program->argc + bin_program->envc + 2;
    
    uint64_t filename = CopyString("/busybox ",bin_program->pagetable,&(bin_program->sp));
    if ((long)filename == -1)
        return -1;

    unsigned char k_rand_bytes[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint64_t u_rand_bytes = bin_program_copy_nbytes(bin_program, (char *)k_rand_bytes, sizeof(k_rand_bytes));
    if ((long)u_rand_bytes == -1)
       return -1;

    uint64_t u_elf_phdr = bin_program_copy_nbytes(bin_program, (char *)elf_phdr, elf->phentsize * elf->phnum);
    printf("u_elf_phdr:%p\n",u_elf_phdr);
    if ((long)u_elf_phdr == -1)
        return -1;

#define NEW_AUX_ENT(id, val)                                                   \
do {                                                                         \
    bin_program->ustack[index++] = id;                                         \
    bin_program->ustack[index++] = val;                                        \
} while (0)
    
  NEW_AUX_ENT(AT_HWCAP, 0x122d);
	NEW_AUX_ENT(AT_PAGESZ, 0x1000);
	NEW_AUX_ENT(AT_CLKTCK, 100);
	NEW_AUX_ENT(AT_PHDR, u_elf_phdr);
	NEW_AUX_ENT(AT_PHENT, sizeof(struct proghdr));
	NEW_AUX_ENT(AT_PHNUM, elf->phnum);
	NEW_AUX_ENT(AT_BASE, 0);
	
  NEW_AUX_ENT(AT_FLAGS, 0);
	NEW_AUX_ENT(AT_ENTRY, elf->entry);
	NEW_AUX_ENT(AT_UID, 0);
	NEW_AUX_ENT(AT_EUID, 0);
	NEW_AUX_ENT(AT_GID, 0);
	NEW_AUX_ENT(AT_EGID, 0);
	NEW_AUX_ENT(AT_SECURE, 0);
	NEW_AUX_ENT(AT_RANDOM, (unsigned long)u_rand_bytes);

    NEW_AUX_ENT(AT_EXECFN, filename);
    NEW_AUX_ENT(AT_PLATFORM, 0);

    NEW_AUX_ENT(AT_BASE_PLATFORM, 0);
    NEW_AUX_ENT(AT_EXECFD, 3);  /* 3 is free */
    NEW_AUX_ENT(AT_NULL, 0);

    #undef NEW_AUX_ENT

    printf("index: %d\n", index);

    bin_program->sp -= sizeof(uint64_t) * index;
    if (copyout(bin_program->pagetable, bin_program->sp,
                (char *)bin_program->ustack, sizeof(uint64_t) * index)) {
        return -1;
    }

    uint64_t argc = bin_program->argc;
    bin_program->sp -= sizeof(uint64_t);
    copyout(bin_program->pagetable, bin_program->sp, (char *)&argc,
            sizeof(uint64_t));
    return 0;
}
int exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint64 /*argc,*/ sz = 0, sp, ustack[MAXARG+1], stackbase;
  struct elfhdr elf;
  struct dirent *ep;
  struct proghdr ph;
  struct proghdr *phdr = kalloc();
  pagetable_t pagetable = 0, oldpagetable;
  pagetable_t kpagetable = 0;//, oldkpagetable;
  bin_program_t bin_prog;
  struct proc *p = myproc();
  printf("enter exec\n");
  int load_addr_set = 0;
  uint64 load_addr = 0;
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
  uint64 vaddr;
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    // va_offset = 0;
    if(eread(ep, 0, (uint64)&ph, off, sizeof(ph)) != sizeof(ph)) {
      goto bad;
    }
    memmove(phdr + i*sizeof(ph), &ph, sizeof(ph));

    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz) {
      goto bad;
    }
    if(ph.vaddr + ph.memsz < ph.vaddr) {
      goto bad;
    }

    uint64 sz1 = 0;
    vaddr = ph.vaddr;
    printf("load vaddr:%p",vaddr);
    if(ph.vaddr % PGSIZE != 0) {
      printf("need align pgsize\n");
      printf("address:%p\n",ph.vaddr);
      vaddr = PGROUNDDOWN(ph.vaddr);
      printf("align address:%p\n",vaddr);
    }

    if((sz1 = uvmalloc(pagetable, kpagetable, sz, vaddr + ph.memsz + sz1)) == 0) {
      goto bad;
    }
    sz = sz1;

    if(!load_addr_set) {
      load_addr_set = 1;
      load_addr = ph.vaddr;
    }
    if(loadseg(pagetable, ph.vaddr, ep, ph.off, ph.filesz) < 0) {
      goto bad;
    }
  }
  eunlock(ep);
  eput(ep);
  ep = 0;
  printf("load_addr:%p\n",load_addr);
  p = myproc();
  uint64 oldsz = p->sz;

  // Allocate two pages at the next page boundary.
  // Use the second as the user stack.
  sz = PGROUNDUP(sz);
  uint64 sz1;
  if((sz1 = uvmalloc(pagetable, kpagetable, sz, sz + PGSIZE * 4)) == 0)
    goto bad;
  sz = sz1;
  //uvmclear(pagetable, sz-4*PGSIZE);
  sp = sz;
  stackbase = sp - PGSIZE * 4;

  bin_program_init(&bin_prog);
  bin_prog.stack_top = 0;
  bin_prog.sp = sp;
  bin_prog.stackbase = stackbase;
  bin_prog.pagetable = pagetable;
  bin_prog.ustack = ustack;
  merge_args((char **)ustack, argv, env);
  bin_prog.argc = bin_program_copy_string2stack(&bin_prog, (char **)ustack);
  if (bin_prog.argc < 0)
      return -1;
  
  bin_prog.envc = bin_program_copy_string2stack(&bin_prog, (char **)ustack);
  if (bin_prog.envc < 0)
      return -1;
  create_user_stack(&bin_prog,&elf,phdr);

  // arguments to user main(argc, argv)
  // argc is returned via the system call return
  // value, which goes in a0.
  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(p->name, last, sizeof(p->name));
    
  // Commit to the user image.
  oldpagetable = p->pagetable;
  //oldkpagetable = p->kpagetable;
  p->pagetable = pagetable;
  p->kpagetable = kpagetable;
  p->sz = sz;
  /*if (path[2] == 'a')
    p->trapframe->epc = 0x0000000000010626;//elf.entry;  // initial program counter = main
  else*/
    p->trapframe->epc = elf.entry;

  printf("path=%s epc:%p sp:%p\n",path,p->trapframe->epc,bin_prog.sp);
  p->trapframe->a1 = bin_prog.sp;
  p->trapframe->ra = 0;
  p->trapframe->a0 = 1;
  p->trapframe->sp = bin_prog.sp; // initial stack pointer
  proc_freepagetable(oldpagetable, oldsz);
  w_satp(MAKE_SATP(p->kpagetable));
  sfence_vma();
  //kvmfree(oldkpagetable, 0);
  //argc = bin_prog.argc;
  printf("argc:%d argv[0]:%s\n",((uint64 *)bin_prog.sp)[0],(char *)(((uint64 *)bin_prog.sp)[1]));
  return bin_prog.argc; // this ends up in a0, the first argument to main(argc, argv)
  //return 0;
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
