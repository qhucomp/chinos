#include "include/kmalloc.h"
#include "include/string.h"
#include "include/printk.h"
#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>
extern char _heap_start[];
char *heap_end = (char *)0x80600000;
struct zone zone_struct = {0,FREE,0};
struct zone *zone_array;
uint64_t *bitmap;
size_t bitmap_size;

void init_kmalloc(void) {
    zone_struct.ptr = _heap_start;
    printk("heap address:%p\n",_heap_start);
    size_t size = 64*sizeof(struct zone);
    size_t bitmap_size = 64*sizeof(uint64_t);
    struct zone *ptr = zone_struct.ptr;
    zone_array = zone_struct.ptr;
    memset(ptr,0,size);
    //初始化bitmap
    bitmap = zone_struct.ptr + size;
    memset(bitmap,0,bitmap_size);

    bitmap[1] = 3;                      //分配了两个struct zone
    bitmap_size = 64;
    struct zone *bitmap_zone = ptr + 1;
    bitmap_zone->ptr = bitmap;
    bitmap_zone->size = bitmap_size;
    bitmap_zone->flag = ALLOC;
    bitmap_zone->next = NULL;
    ptr->ptr = ptr;
    ptr->flag = ALLOC;
    ptr->next = bitmap_zone;
    zone_struct.next = ptr;
    zone_struct.ptr = (char *)zone_struct.ptr + size + bitmap_size;
    zone_struct.size = heap_end - _heap_start;
    printk("kmalloc init.........OK\n");
}
struct zone *alloc_zone(void) {
    for(size_t i = 0;i < bitmap_size;i++)
        if (~(bitmap[i] & 0xffffffffffffffff))
            for(int j = 0;j < 64;j++)
                if (bitmap[i] & (1 << j))
                    return &zone_array[i + j];
    return NULL;
}

void *kmalloc(size_t size) {
    for (struct zone *ptr = &zone_struct;ptr->next != NULL;ptr = ptr->next) {
        if ((ptr->flag == FREE) && (ptr->size > size)) {
            struct zone *zone_ptr = alloc_zone();
            void *p = ptr->ptr;
            ptr->ptr = (char *)ptr->ptr + size;
            ptr->size -= size;
            zone_ptr->ptr = p;
            zone_ptr->size = size;
            zone_ptr->flag = ALLOC;
            zone_ptr->next = ptr->next;
            ptr->next = zone_ptr;
            return p;
        } else if ((ptr->flag == FREE) && (ptr->size == size)) {
            ptr->flag = ALLOC;
            return ptr->ptr;
        }
    }
    return NULL;
}

void free_zone(struct zone *ptr) {
    size_t index = (ptr - zone_array) / sizeof(struct zone);
    size_t i,j;
    i = index / 64;
    j = index % 64;
    if (i < bitmap_size)
        //清空位
        bitmap[i] = bitmap[i] & ~(1 << j);
    else
        printk("free_zone error!\n");
}
void kfree(void *ptr) {
    /*struct zone *prev,*next;
    struct zone *temp;
    for (struct zone *p = &zone_struct;p->next != NULL;p = p->next) {
        if (p->ptr == ptr) {
            p->flag = FREE;
            if (prev->flag == FREE) {
                p->ptr = prev->ptr;
                p->size += prev->size;
                p-
            }
        } else if (((void *)((char *)p->ptr + p->size)) == ptr) {   //合并内存
            prev = p;
        } else if ((void *)((char *)ptr + p->size) == p->ptr) {
            next = p;
        }
        temp = p;
    }*/
    for (struct zone *p = &zone_struct;p->next != NULL;p = p->next) {
        if (p->ptr == ptr) {
            p->flag = FREE;
            return;
        }
    }
    printk("kfree error\n");
}