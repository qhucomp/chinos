#include "include/user.h"
#include "include/riscv.h"
#include "include/task.h"
#include "include/vfs.h"
#include "include/printk.h"
#include "include/string.h"
#include "include/kmalloc.h"
#include "include/elf_parse.h"
#include "include/syscalls.h"
#include "include/ecall.h"
void user_exit(void) {
    ECALL(SYS_exit,0,0,0,0,0,0);
}
char elf[72704];
task_struct *user_thread(const char *name) {
    task_struct *task = alloc_task(current);
    void *user_space = user_malloc(task->pid);
    //printk("start open\n");
    dentry_struct *p = vfs_open(NULL,name);
    // p = vfs_open(NULL,name);
    // char *elf = kmalloc(p->file_size);
    //printk("open ok\n");
    printk("file size:%d\n",p->file_size);
    if (!p->file_size)
        panic("read zero");
    memset(elf,0,p->file_size);
    // printk("read ok!\n");
    vfs_read(p,elf,p->file_size);
    // vfs_read(p,elf,p->file_size);
    //printk("elf ptr:%p p->file_size:%d\n",elf,p->file_size);
    size_t size = 0;
    uint64_t offset;
    // 构建进程在内存中的镜像
    memset(user_space,0,USER_HEAP_SIZE);
    // memset((void *)0x80200000,0,0x400000);
    size += copy_section(elf,user_space,".text",&offset);
    //printk("offset:%p\n",offset);
    task->task_reg.x6 = task->epc = (uintptr_t)user_space + offset;
    task->task_reg.x1 = (uintptr_t)user_exit;
    size += copy_section(elf,user_space,".rodata",NULL);
    size += copy_section(elf,user_space,".data",NULL);
    size += copy_section(elf,user_space,".srodata",NULL);
    task->flag |= TASK_FLAG_USER_THREAD;
    task->task_reg.x11 = task->sp = (uintptr_t)user_space + USER_HEAP_SIZE;
    task->brk_base = 0;
    add_task(task);
    //printk("task epc:%p\n",task->epc);
    return task;
}

void *user_malloc(pid_t pid) {
    uintptr_t user_process_address = 0x80400000 + 0x1000;
    return (void *)(user_process_address + (pid - 1) * USER_HEAP_SIZE);
}