#include <stdint.h>
#include "include/sleep.h"
#include "include/sysctl.h"
#include "include/riscv.h"

int usleep(uint64_t usec)
{
    uint64_t cycle = read_cycle();
    uint64_t nop_all = usec * sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 1000000UL;
    while(1)
    {
        if(read_cycle() - cycle >= nop_all)
            break;
    }
    return 0;
}