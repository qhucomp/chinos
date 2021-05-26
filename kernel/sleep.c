#include <stdint.h>
#include "include/sleep.h"
#include "include/sysctl.h"
#include "include/riscv.h"
#include "include/syscalls.h"

int usleep(uint64_t usec)
{
    uint64_t cycle = read_cycle();
    uint64_t nop_all = usec * sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 1000000UL;
    while(1)
    {
        if(read_cycle() - cycle >= nop_all)
            break;
    }
    global_tms.tms_utime += usec;
    if(global_tms.tms_utime >= 1000) {
        global_tms.tms_stime += global_tms.tms_utime/1000;
        global_tms.tms_utime -= (usec/1000)*1000;
    }
    return 0;
}