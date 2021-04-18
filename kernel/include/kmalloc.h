#ifndef __KMALLOC_H
#define __KMALLOC_H
#include <stddef.h>
#include <stdint.h>
struct zone {
    void *ptr;
    size_t size;
    int flag;
    struct zone *next;
};
#define ALLOC   1
#define FREE    0
void init_kmalloc(void);
struct zone *alloc_zone(void);
void free_zone(struct zone *ptr);
void *kmalloc(size_t size);
void kfree(void *ptr);
extern struct zone zone_struct;
extern struct zone *zone_array;
extern uint64_t *bitmap;
extern size_t bitmap_size;
#endif