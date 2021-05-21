#include "include/user.h"
#include "include/riscv.h"
#include "include/task.h"
#include "include/vfs.h"
#include "include/printk.h"
#include "include/string.h"
#include "include/kmalloc.h"
#include "include/elf_parse.h"

void user_thread(const char *name) {
    task_struct *task = alloc_task(current);
    printk("task->pid=%d\n",task->pid);
    void *user_space = user_malloc(task->pid);
    memset(user_space,0,USER_HEAP_SIZE);
    printk("user_space:%p\n",user_space);
    dentry_struct *p = vfs_open(name);
    char *elf = kmalloc(p->file_size);
    printk("vfs_open OK!\n");
    vfs_read(p,elf,p->file_size);
    size_t size = 0;

    // 构建进程在内存中的镜像
    size += copy_section(elf,user_space,".text");
    task->epc = (uintptr_t)user_space + size;

    size += copy_section(elf,user_space,".rodata");
    size += copy_section(elf,user_space,".data");
    size += copy_section(elf,user_space,".srodata");

    task->flag |= TASK_FLAG_USER_THREAD;
    task->task_reg.x11 = task->sp = (uintptr_t)(user_space + USER_HEAP_SIZE);
    task->brk_base = (uintptr_t)user_space + size;
    add_task(task);
    printk("user sp:%p\n",task->pid,task->sp);
}

void *user_malloc(pid_t pid) {
    uintptr_t user_process_address = 0x80200000 + 0x1000;
    return (char *)(user_process_address + (pid - 2) * USER_HEAP_SIZE);
}