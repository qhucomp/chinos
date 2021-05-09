#include "include/vfs.h"
#include "include/kmalloc.h"
#include "include/string.h"
#include "include/printk.h"
#include <stdint.h>

dentry_struct *create_dentry(void) {
    dentry_struct *ptr = kmalloc(sizeof(dentry_struct));
    if (!ptr)
        return NULL;
    memset(ptr,0,sizeof(dentry_struct));
    ptr->sectorno_list = kmalloc(sizeof(uint32_t)*DEFAULT_SECTOR_LIST_SIZE);
    if(!ptr->sectorno_list) {
        kfree(ptr);
        return NULL;
    }
    memset(ptr->sectorno_list,0,sizeof(uint32_t)*DEFAULT_SECTOR_LIST_SIZE);
    ptr->sectorno_list_len = DEFAULT_SECTOR_LIST_SIZE;
    return ptr;
}

static void check_sectorno_list(dentry_struct *dentry) {
    if (!(dentry->sector_count >= dentry->sectorno_list_len))
        return;
    uint32_t *temp = kmalloc(sizeof(uint32_t) * (dentry->sectorno_list_len + DEFAULT_SECTOR_LIST_SIZE));
    if(!temp) {
        printk("OOM!\n");
        while(1);
    }
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