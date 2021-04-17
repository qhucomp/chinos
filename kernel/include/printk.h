#ifndef __PRINTK_H
#define __PRINTK_H
#include <stdarg.h>
int printk(const char *fmt,...);
int vfprintk(const char *fmt,va_list ap);
#endif