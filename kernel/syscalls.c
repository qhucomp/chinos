#include <stdint.h>
#include "include/trap.h"
#include "include/syscalls.h"
#include "include/uart.h"
#include "include/printk.h"
#include "include/task.h"
#include "include/kmalloc.h"
#include "include/string.h"

syscall_func syscalls[300];

static int get_new_fd(void) {
        for(int j = 0;j < 32;j++)
            if (!(current->fd_bitmap & (1U << j))) {
                current->fd_bitmap |= 1U << j;
                return j + 1;
            }
    return -1;
}

ssize_t sys_read(int64_t fd,void *buf,size_t count) {
    ssize_t result = 0;
    if (current->entry[fd - 2]) {
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
    dentry_struct *p = vfs_open(_p);
    current->entry[fd - 2] = p;
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
            return 0;
        case SYS_exit:
            sys_exit(reg->x10);
            while(1);
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

uintptr_t sys_brk(size_t pos) {
    current->brk_base += pos;
    return current->brk_base;
}

void sys_exit(int code) {
    delete_task(current);
    current = current->parent;
    return;
}

void register_syscall(void) {
    syscalls[SYS_write] = (syscall_func)sys_write;
    syscalls[SYS_openat] = (syscall_func)sys_openat;
    syscalls[SYS_read] = (syscall_func)sys_read;
}