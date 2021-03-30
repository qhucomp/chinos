#ifndef CHINO_USER_SYSCALL
#define CHINO_USER_SYSCALL
#include <stdint.h>
/*
 * 调用系统调用
 * @syscall_no:系统调用号
 * @param1:系统调用参数1
 * @param2:系统调用参数2
 * @param3:系统调用参数3
 * @param4:系统调用参数4
 * @param5:系统调用参数5
 * @param6:系统调用参数6
 * @return:返回一个void *指针，值由系统调用确定
 */
void *syscall(int syscall_no, uint64_t param1, uint64_t param2, uint64_t param3,
              uint64_t param4, uint64_t param5, uint64_t param6);

#endif
