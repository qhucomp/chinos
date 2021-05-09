#ifndef __STRING_H
#define __STRING_H
#include <stddef.h>
/**
 * @brief 初始化内存区域
 * 
 * @param[in] __dst 被初始化的内存地址
 * 
 * @param[in] c 内存区域初始化的值
 * 
 * @param[in] count 内存大小
 */
void *memset(void *__dst,int c,size_t count);

void *memcpy(void *__dest,const void *__src,size_t n);

int strncmp(const char *s1,const char *s2,size_t n);

size_t strlen(const char *s1);
#endif