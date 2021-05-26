#ifndef __KALLOC_H
#define __KALLOC_H
#include <stdint.h>
#define PHYSTOP 0x80300000
#define PGSIZE 8192
#define PGSHIFT 12

void* kalloc(void);
void _kfree(void *);
void kinit(void);
uint64_t freemem_amount(void);
#endif
