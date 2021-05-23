#include "include/kmalloc.h"
#include "include/string.h"
#include "include/printk.h"
#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>
extern char _heap_start[];
char *kernel_heap_end = (char *)0x80200000;
struct zone zone_struct = {0,FREE,0};
struct zone *zone_pointer;
struct zone *zone_array;
uint64_t *bitmap;
size_t bitmap_size;
int total_size;
void init_kmalloc(void) {
    zone_struct.ptr = (void *)_heap_start;//_heap_start  + 4096*16;
    printk("heap address:%p\n",_heap_start);
    size_t zone_array_size = 256*sizeof(struct zone);
    bitmap_size = 4;
    bitmap = zone_struct.ptr;

    memset(bitmap,0,sizeof(uint64_t)*bitmap_size);
    zone_array = (void *)((char *)zone_struct.ptr + sizeof(uint64_t)*bitmap_size);
    zone_struct.ptr = _heap_start + sizeof(uint64_t)*bitmap_size + zone_array_size;
    zone_struct.size = (size_t)kernel_heap_end - (size_t)zone_struct.ptr;
    zone_pointer = &zone_struct;
    printk("kernel heap size:%ld\n",zone_struct.size,zone_struct.ptr);
    printk("kmalloc init.........OK\n");
}
static void mem_block_merge(void) {
    //合并内存块，将小内存合并为大内存
    struct zone *ptr = zone_pointer;
    for(;ptr != NULL;ptr = ptr->next)
        if (ptr->flag == FREE)
            break;
    
    for(;ptr!=NULL;ptr=ptr->next)
    {
        if((ptr->flag==FREE)&&(ptr->next->flag==FREE)&&(ptr->next!=NULL))
        {
            
            ptr->next->size+=ptr->size;
            ptr->next->before=ptr->before;
            struct zone *tem=ptr;
            ptr=ptr->next;       
            ptr->before->next=ptr;    
            free_zone(tem);
           // ptr->before->next=ptr;
        }
        else
            break;
    }
    /*for(;ptr != NULL;ptr = ptr->next) {
        if ((ptr->flag == FREE) && (ptr->next != NULL) && (ptr->next->flag == FREE)) {
            ptr->size += ptr->next->size;
           // struct zone *tem=ptr-
            
            
            ptr->next = ptr->next->next;
            free_zone(ptr->next);
        } else
            break;
    }*/
}
struct zone *alloc_zone(void) {
start:
    for(size_t i = 0;i < bitmap_size;i++) {
        if (~(bitmap[i] & 0xffffffffffffffff)) {
            for(int j = 0;j < 64;j++)
                if (!(bitmap[i] & (1UL << j))) {
                    bitmap[i] |= 1UL << j;
                    return &zone_array[i + j];
                }
        }
    }
    mem_block_merge();
        goto start;
    return NULL;
}



void *_kmalloc(size_t size) {
    int flag = 1;
start:
    for (struct zone *ptr = zone_pointer;ptr != NULL;ptr = ptr->next) {
        //printk("size:%d ptr->size:%d flag:%d ptr:%p\n",size,ptr->size,ptr->flag,ptr->ptr);
        if ((ptr->flag == FREE) && (ptr->size > size)) {
            char *p = ptr->ptr;
            // 地址对齐
            // 存在内存泄漏
            while((uintptr_t)p % 8 != 0) {
                if ((p - (char *)ptr->ptr) > size)
                    goto loop_end;
                p++;
            }
            struct zone *zone_ptr = alloc_zone();
            if (!zone_ptr)
                break;
            ptr->ptr = (char *)ptr->ptr + size;
            //printk("after ptr:%p\n",ptr->ptr);
            ptr->size -= size;
            total_size += size;
            zone_ptr->ptr = p;
            zone_ptr->size = size;
            zone_ptr->flag = ALLOC;
            zone_ptr->next = zone_pointer;
            //zone_ptr->next=ptr;
            zone_pointer = zone_ptr;
            zone_ptr->next->before=zone_ptr;//建立双向链表
            //printk("%p OK!\n",zone_ptr);
            return p;
        }
        else if ((ptr->flag == FREE) && (ptr->size == size)) {
            ptr->flag = ALLOC;
            total_size += size;
            return ptr->ptr;
        }
loop_end: ;
    }

    if (flag) {
        flag = 0;
        mem_block_merge();
        goto start;
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
        panic("index error!\n");
}

void kfree(void *ptr) {
    if(!ptr)
        return;
    for (struct zone *p = zone_pointer;p->next != NULL;p = p->next) {
        if (p->ptr == ptr) {
            p->flag = FREE;
            return;
        }
    }
    printk("pointer address:%p\n",ptr);
    panic("no pointer\n");
}