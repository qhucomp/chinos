#include <stdint.h>
#include "include/trap.h"
#include "include/syscalls.h"
#include "include/uart.h"
#include "include/printk.h"
#include "include/task.h"
#include "include/kmalloc.h"
#include "include/string.h"
#include "include/user.h"
#include "include/shell.h"
#include "include/sleep.h"
syscall_func syscalls[300];
struct tms global_tms;
static int get_new_fd(void) {
        for(int j = 0;j < 32;j++)
            if (!(current->fd_bitmap & (1U << j))) {
                current->fd_bitmap |= 1U << j;
                return j + 1;
            }
    return -1;
}
void sys_user_task(const char *path);
ssize_t sys_read(int64_t fd,void *buf,size_t count) {
    ssize_t result = 0;
    //printk("syscall read %d %p\n",fd,current->entry[fd - 2]);
    if (current->entry[fd - 2]) {
        //printk("ok\n");
        result = vfs_read(current->entry[fd - 2],buf,count);
    }
    return result;
}

int sys_openat(int64_t dirfd,const char *path,int flags) {
    int fd = get_new_fd();
    char *_p;
    size_t len = strlen(path) + strlen(current->work_dir) + 1;
    _p = kmalloc(len);
    if(!_p)
        panic("out of memory");
    memset(_p,0,len);
    strncat(_p,current->work_dir,len);
    strncat(_p,path,len);
    dentry_struct *p;
    if (dirfd == AT_FDCWD)
        p = vfs_open(NULL,_p);
    else
        p = vfs_open(current->entry[dirfd],path);

    current->entry[fd - 2] = p;
    //printk("fd:%d %p\n",fd,current->entry[fd - 2]);
    p->flags = flags;
    if(!p)
        return -1;
    return fd;
}

static void putchar(char ch) {
    uart_send(ch);
}

uintptr_t handle_ecall(uint64_t extension,regs *reg) {
    switch(extension) {
        case SYS_write:
            return sys_write(reg->x10,(void *)reg->x11,reg->x12);
        case SYS_openat:
            return sys_openat(reg->x10,(void *)reg->x11,reg->x12);
        case SYS_read:
            return sys_read(reg->x10,(void *)reg->x11,reg->x12);
        case SYS_close:
            return sys_close(reg->x10);
        case SYS_exit:
            return sys_exit(reg->x10);
        case SYS_brk:
            return sys_brk(reg->x10);
        case SYS_wait4:
            return sys_wait4(reg->x10,(int *)reg->x11,reg->x12);
        case SYS_clone:
            return sys_clone(reg->x10,reg->x11,reg->x12,reg->x13,reg->x14);
        case SYS_user_task:
            sys_user_task((char *)reg->x10);
            return 0;
        case SYS_uname:
            sys_uname((void *)reg->x10);
            return 0;
        default:
            return 0;
    }
}




ssize_t sys_write(int fd,void *buf,size_t count) {
    ssize_t result = 0;
    if (fd == 1) {
        for(size_t i = 0;i < count;i++,result++)
            putchar(((char *)buf)[i]);
        goto end;
    }
end: ;
    return result;
}

intptr_t sys_brk(size_t pos) {
    if (pos)
        current->brk_base = pos;
    //printk("pos:%p %p\n",current->brk_base,pos);
    return current->brk_base;
}

int sys_exit(int code) {
    delete_task(current);
    current->exit_code = code;
    // for(int i = 0;i < current->parent->chilren_len;i++) {
    //     if (current->parent->chilren[i] && current->parent->chilren[i]->status != TASK_DIE) {
    //         current = current->parent->chilren[i];
    //         return current->task_reg.x10;
    //     }
    // }
    current = current->parent;
    //printk("pid:%d %p\n",current->pid,current->epc);
    // if (current->pid == 0)
    //     current->epc = (uintptr_t)user_shell - 4;
    //printk("pid:%d ret:%d\n",current->pid,current->task_reg.x10);
    current->task_reg.x10 = current->pid;
    current->status &= ~TASK_FLAG_FORK;
    return current->task_reg.x10;
}

int sys_clone(int flags,uintptr_t stack,pid_t ptid,int tls,int ctid) {
    if (current->pid == 0)
        return 0;
    task_struct *task = alloc_task(current);
    void *user_space = user_malloc(task->pid);
    task->task_reg = current->task_reg;
    memcpy(user_space,get_user_space(current->pid),USER_HEAP_SIZE);
    uint64_t offset = current->epc - (uint64_t)get_user_space(current->pid);
    //printk("fork!\n");
    //printk("pid:%d offset:%p %p\n",current->pid,current->epc,(uint64_t)get_user_space(current->pid));
    task->epc = (uintptr_t)user_space + offset;
    offset = current->task_reg.x2 - (uint64_t)get_user_space(current->pid);
    task->sp = task->task_reg.x11 = (uintptr_t)user_space + offset;

    task->task_reg.x10 = 0;
    current->task_reg.x10 = task->pid;
    //printk("fork pid:%d\n",task->pid);
    task->status = current->status;
    current->status |= TASK_FLAG_FORK;
    add_task(task);
    current = task;
    return 0;
}

int sys_wait4(pid_t pid,int *status,int options) {
    int result = 0;
    //printk("wait1!\n");
    //printk("wait %d\n",pid == -1);
    for(int i = 0;i < current->chilren_len;i++) {
        //printk("current->chilren[%d]=%p\n",i,current->chilren[i]);
        if (current->chilren[i] && current->chilren[i]->status == TASK_DIE && pid == -1) {
            *status = current->chilren[i]->exit_code;
            kfree(current->chilren[i]);
            current->chilren[i] = NULL;
            //printk("wait!\n");
            break;
        } else if (current->chilren[i] && current->chilren[i]->status == TASK_DIE && current->chilren[i]->pid == pid) {
            result = current->chilren[i]->exit_code;
            kfree(current->chilren[i]);
            current->chilren[i] = NULL;
            break;
        }
    }
    return result;
}


void sys_user_task(const char *path) {
    if(path == NULL)
        return;
    printk("path:%s\n",path);
    task_struct *task = user_thread(path);
    current->status |= TASK_FLAG_FORK;
    current = task;
    current->epc -= 4;
    //printk("mepc:%p\n",current->epc);
}

void sys_uname(struct utsname *ptr) {
    memcpy(ptr->sysname,"chino os",8);
    memcpy(ptr->nodename,"chino",5);
    memcpy(ptr->release,"0",1);
    memcpy(ptr->version,"0.01",4);
    memcpy(ptr->machine,"riscv64",7);
    memcpy(ptr->domainname,"chino",5);
}

int sys_close(uint64_t fd) {
    if(current->entry[fd]) {
        free_dentry(current->entry[fd]);
        current->entry[fd] = NULL;
        return 0;
    }
    return -1;
}

int sys_times(struct tms *ptr) {
    memcpy(ptr,&global_tms,sizeof(struct tms));
    return 0;
}

int sys_sleep(uint64_t sec) {
    usleep(sec);
    return 0;
}


void register_syscall(void) {
    syscalls[SYS_write] = (syscall_func)sys_write;
    syscalls[SYS_openat] = (syscall_func)sys_openat;
    syscalls[SYS_read] = (syscall_func)sys_read;

}