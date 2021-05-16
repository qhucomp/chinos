#include "include/string.h"
#include <stddef.h>
#include "include/printk.h"
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
    size_t i;
    for(i = 0;i < n;i++) {
        if (s1[i] == '\0' || s2[i] == '\0')
            break;
        if (s1[i] != s2[i]) 
            break;
    }
    if (i == n)
        return 0;
    return s1[i] - s2[i];
}

size_t strlen(const char *s1) {
    size_t len;
    for(len = 0;s1[len] != '\0';len++);
    return len; 
}

char *strncat(char *__dst,const char *__src,size_t n) {
    size_t len = strlen(__dst);
    size_t src_len = strlen(__src);
    memcpy(__dst + len,__src,src_len);
    return __dst;
}

char *strupr(char *str){
    char *orign=str;
    for (; *str!='\0'; str++) {
        if (*str >= 97 && *str <= 122)
            *str = *str - 32;
    }
    return orign;
}
 char *strchr(const char *s, int c) {
     size_t i;
     for(i = 0;s[i] != '\0' && s[i] != c;i++);
     return (char *)s + i;
 }