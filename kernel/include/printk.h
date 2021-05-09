#ifndef __PRINTK_H
#define __PRINTK_H
#include <stdarg.h>

/**
 * @brief 内核态打印函数
 * @param[in] fmt 被格式化的字符串
 * @param[in] ... 要格式化的参数
 */
int printk(const char *fmt,...);

/**
 * @brief 将字符串格式化并输出到串口
 * @param[in] fmt 被格式化的字符串
 * @param[in] ap 被格式化的参数
 */
int vfprintk(const char *fmt,va_list ap);

/**
 * @brief 引发异常
 */
#define panic(message)                                                  \
({                                                                      \
    printk("(%s:%d)%s:%s\n",__FILE__,__LINE__,__func__,message);        \
    while(1);                                                           \
})
#endif