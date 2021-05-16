#include "include/vfs.h"
#include "include/kmalloc.h"
#include "include/string.h"
#include "include/printk.h"
#include "include/fat32.h"
#include <stdint.h>

dentry_struct *create_dentry(void) {
    dentry_struct *ptr = kmalloc(sizeof(dentry_struct));
    if (!ptr)
        return NULL;

    memset(ptr,0,sizeof(dentry_struct));
    ptr->sectorno_list = kmalloc(sizeof(uint32_t)*DEFAULT_SECTOR_LIST_SIZE);
    memset(ptr->sectorno_list,0,sizeof(uint32_t)*DEFAULT_SECTOR_LIST_SIZE);
    ptr->sectorno_list_len = DEFAULT_SECTOR_LIST_SIZE;
    //while(1);
    return ptr;
}

static void check_sectorno_list(dentry_struct *dentry) {
    printk("dentry->sector_count=%d dentry->sectorno_list_len=%d\n",dentry->sector_count,dentry->sectorno_list_len);
    if (!(dentry->sector_count >= dentry->sectorno_list_len))
        return;
    uint32_t *temp = kmalloc(sizeof(uint32_t) * (dentry->sectorno_list_len + DEFAULT_SECTOR_LIST_SIZE));
    if(!temp)
        panic("out of memory!");
    memcpy(temp,dentry->sectorno_list,sizeof(uint32_t)*dentry->sectorno_list_len);
    kfree(dentry->sectorno_list);
    dentry->sectorno_list = temp;
    dentry->sectorno_list_len += DEFAULT_SECTOR_LIST_SIZE;
}

void push_sectorno(dentry_struct *dentry,uint32_t sectorno) {
    check_sectorno_list(dentry);
    dentry->sectorno_list[dentry->sector_count++] = sectorno;
}

void free_dentry(dentry_struct *ptr) {
    if(!ptr)
        return;
    kfree(ptr->sectorno_list);
    if(ptr->name)
        kfree(ptr->name);
    kfree(ptr->sectorno_list);
}

size_t vfs_read(dentry_struct *p,void *buf,size_t size) {
    size_t result = 0;
    result = fat32_read(p,buf,size);
    return result;
}

dentry_struct *vfs_open(const char *name) {
    return fat32_open(name);
}