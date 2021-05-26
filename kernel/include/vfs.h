#ifndef __FILE_SYSTEM_H
#define __FILE_SYSTEM_H
#include <stdint.h>
#include <stddef.h>
typedef struct __dentry {
    uint32_t *sectorno_list;
    uint32_t sectorno_list_len;
    uint32_t sector_count;
    uint32_t size;
    uint32_t file_size;
    int type;
    uint64_t flags;
    char *name;
} dentry_struct;
size_t vfs_read(dentry_struct *p,void *buf,size_t size);
dentry_struct *vfs_open(dentry_struct *dir,const char *name);
dentry_struct *create_dentry(void);
//void check_sectorno_list(dentry_struct *dentry);
void push_sectorno(dentry_struct *dentry,uint32_t sectorno);
void free_dentry(dentry_struct *ptr);
#define DEFAULT_SECTOR_LIST_SIZE 128
#define O_DIRECTORY 0x0200000
#endif