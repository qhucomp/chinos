#include "include/riscv.h"
#include "include/printk.h"
#include <stdint.h>
void set_mpp(uint64_t mpp) {
    uint64_t _mstatus = read_csr(mstatus);
    _mstatus &= ~(3UL << 11);
    //if (mpp != 0)
    //    _mstatus |= mpp;
    _mstatus |= mpp;
    write_csr(mstatus,_mstatus);
}