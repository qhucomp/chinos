#include "include/string.h"
#include <stddef.h>

void *memset(void *__dst,int c,size_t count) {
    char *dst = __dst;
    for(size_t i = 0;i < count;i++)
        dst[i] = c;
    return __dst;
}

void *memcpy(void *__dest,const void *__src,size_t n) {
    char *dst = __dest;
    for(size_t i = 0;i < n;i++)
        dst[i] = *(char *)(__src + i);
    return __dest;
}

int strncmp(const char *s1,const char *s2,size_t n) {
    for(size_t i = 0;i < n;i++) {
        if (s1[i] == s2[1])
            continue;
        return s1[i] - s2[i];
    }
    return 0;
}

size_t strlen(const char *s1) {
    size_t len;
    for(len = 0;s1[len] != 0;len++);
    return len; 
}