#include <stdint.h>
#include "include/kalloc.h"
#include "include/printk.h"
#include "include/string.h"
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))

void freerange(void *pa_start, void *pa_end);

extern char _heap_start[]; // first address after kernel.

struct run {
  struct run *next;
};

struct {
  struct run *freelist;
  uint64_t npage;
} kmem;


void kinit(void)
{
  kmem.freelist = 0;
  kmem.npage = 0;
  freerange(_heap_start, (void*)PHYSTOP);
  #ifdef DEBUG
  printf("kernel_end: %p, phystop: %p\n", kernel_end, (void*)PHYSTOP);
  printf("kinit\n");
  #endif
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64_t)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    _kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void _kfree(void *pa)
{
  struct run *r;
  
  if(((uint64_t)pa % PGSIZE) != 0 || (char*)pa < _heap_start || (uint64_t)pa >= PHYSTOP) {
      //printk("%p %d %d %d\n",pa,((uint64_t)pa % PGSIZE) != 0,(char*)pa < _heap_start,(uint64_t)pa >= PHYSTOP);
    panic("kfree");
  }
  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  r->next = kmem.freelist;
  kmem.freelist = r;
  kmem.npage++;
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *kalloc(void)
{
  struct run *r;

  r = kmem.freelist;
  if(r) {
    kmem.freelist = r->next;
    kmem.npage--;
  }

//   if(r)
//     memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

uint64_t freemem_amount(void)
{
  return kmem.npage << PGSHIFT;
}