#include "include/kmalloc.h"
#include "include/string.h"
#include "include/printk.h"
#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>
extern char _heap_start[];
char *kernel_heap_end = (char *)0x80400000;
struct zone zone_struct = {0,FREE,0};
struct zone *zone_array;
uint64_t *bitmap;
size_t bitmap_size;

void init_kmalloc(void) {
    zone_struct.ptr = _heap_start;
    printk("heap address:%p\n",_heap_start);
    size_t size = 64*sizeof(struct zone);
    size_t _bitmap_size = 64*sizeof(uint64_t);
    struct zone *ptr = zone_struct.ptr;
    zone_array = zone_struct.ptr;
    memset(ptr,0,size);
    //初始化bitmap
    bitmap = zone_struct.ptr + size;
    memset(bitmap,0,_bitmap_size);

    bitmap[1] = 3;                      //分配了两个struct zone
    bitmap_size = 64;
    struct zone *bitmap_zone = ptr + 1;
    bitmap_zone->ptr = bitmap;
    bitmap_zone->size = _bitmap_size;
    bitmap_zone->flag = ALLOC;
    bitmap_zone->next = NULL;
    ptr->ptr = ptr;
    ptr->flag = ALLOC;
    ptr->next = bitmap_zone;
    zone_struct.next = ptr;
    zone_struct.ptr = (void *)((char *)zone_struct.ptr + size + _bitmap_size);
    zone_struct.size = kernel_heap_end - _heap_start - size - _bitmap_size;
    printk("kernel heap size:%ld\n",zone_struct.size,zone_struct.ptr);
    printk("kmalloc init.........OK\n");
}

struct zone *alloc_zone(void) {
    for(size_t i = 0;i < bitmap_size;i++) {
        if (~(bitmap[i] & 0xffffffffffffffff)) {
            for(int j = 0;j < 64;j++)
                if (bitmap[i] & (1 << j)) {
                    bitmap[i] |= 1 << j;
                    return &zone_array[i + j];
                }
        }
    }
    return NULL;
}

static void mem_block_merge(void) {
    //合并内存块，将小内存合并为大内存
    for(struct zone *ptr = &zone_struct;ptr->next != NULL;ptr = ptr->next) {
        if ((ptr->flag == FREE) && (ptr->next != NULL) && (ptr->next->flag == FREE)) {
            ptr->size += ptr->next->size;
            ptr->next = ptr->next->next;
            free_zone(ptr->next);
        }
    }
}

void *kmalloc(size_t size) {
    int flag = 1;
    struct zone *prev_ptr = &zone_struct;
start:
    for (struct zone *ptr = &zone_struct;ptr->next != NULL;ptr = ptr->next) {
        if ((ptr->flag == FREE) && (ptr->size > size)) {
            struct zone *zone_ptr = alloc_zone();
            char *p = ptr->ptr;
            ptr->ptr = (char *)ptr->ptr + size;
            ptr->size -= size;

            //使分配的内存地址为递增
            zone_ptr->ptr = p;
            zone_ptr->size = size;
            zone_ptr->flag = ALLOC;
            zone_ptr->next = ptr;
            prev_ptr->next = zone_ptr;
            return p;
        } else if ((ptr->flag == FREE) && (ptr->size == size)) {
            ptr->flag = ALLOC;
            return ptr->ptr;
        }
        prev_ptr = ptr;
    }

    if (flag) {
        flag = 0;
        mem_block_merge();
        goto start;
    }
    printk("kmalloc():out of memory!\n");
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
        printk("free_zone() error!\n");
}

void kfree(void *ptr) {
    for (struct zone *p = &zone_struct;p->next != NULL;p = p->next) {
        if (p->ptr == ptr) {
            p->flag = FREE;
            return;
        }
    }
    printk("kfree() error\n");
}