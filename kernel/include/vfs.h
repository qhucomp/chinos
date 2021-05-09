#ifndef __FILE_SYSTEM_H
#define __FILE_SYSTEM_H
#include "fat32.h"
typedef struct __superblock {
    int type;
    union {
        fat32_fs *fat32;
    };
} superblock;
typedef struct __dentry {
    uint32_t *sectorno_list;
    uint32_t sectorno_list_len;
    uint32_t sector_count;
    uint32_t size;
    int type;
    char *name;
} dentry_struct;

size_t superblock_write(superblock *superblock,const char *path,const void *data,size_t size);
size_t superblock_read(superblock *superblock,const char *path,void *buf,size_t size);
dentry_struct *create_dentry(void);
//void check_sectorno_list(dentry_struct *dentry);
void push_sectorno(dentry_struct *dentry,uint32_t sectorno);
void free_dentry(dentry_struct *ptr);
#define DEFAULT_SECTOR_LIST_SIZE 128
#endif