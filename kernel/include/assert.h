#ifndef __ASSERT_H
#define __ASSERT_H
#define assert(expression)                                  \
{                                                           \
    if (!(expression)) {                                    \
        printk("%s:%s %d\n",__FILE__,__func__,__LINE__);    \
        while(1);                                           \
    }                                                       \
}
#endif