#include "include/string.h"
#include <stddef.h>

void *memset(void *__dst,int c,size_t count) {
    char *dst = __dst;
    for(size_t i = 0;i < count;i++)
        dst[i] = c;
    return __dst;
}