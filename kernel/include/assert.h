#ifndef __ASSERT_H
#define __ASSERT_H
#include "printk.h"
#define assert(expression)                                                      \
{                                                                               \
    if (!(expression)) {                                                        \
        printk("(%s:%s):%d %s\n",__FILE__,__func__,__LINE__,#expression);       \
        while(1);                                                               \
    }                                                                           \
}
#endif