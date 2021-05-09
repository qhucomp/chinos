#ifndef __KMALLOC_H
#define __KMALLOC_H
#include <stddef.h>
#include <stdint.h>
/**
 * @brief 管理kmalloc()行为的结构
 */
struct zone {
    void *ptr;
    size_t size;
    int flag;
    struct zone *next;
};

//内存块已经分配
#define ALLOC   1

//内存块未分配
#define FREE    0

#define DEFAULT_BITMAP_SIZE (32)
/**
 * @brief 初始化kmalloc内存分配器
 */
void init_kmalloc(void);

/**
 * @brief 分配一个struct zone
 * 
 * @return 返回一个struct zone指针
 */
struct zone *alloc_zone(void);

/**
 * @brief 释放一个struct zone
 */
void free_zone(struct zone *ptr);

/**
 * @brief 从zone里分配内存
 * 
 * @param[in] size  要分配的内存大小
 * 
 * @return 返回已经分配好的内存地址
 */
void *_kmalloc(size_t size);

#define kmalloc(size)                                                           \
({                                                                              \
    printk("%s %s %d\n",__FILE__,__func__,__LINE__);                            \
    void *ptr = _kmalloc(size);                                                 \
    ptr;                                                                        \
})
/**
 * @brief 释放从kmalloc里分配的内存
 */
void kfree(void *ptr);

extern struct zone zone_struct;
extern struct zone *zone_array;

extern uint64_t *bitmap;
extern size_t bitmap_size;
#endif