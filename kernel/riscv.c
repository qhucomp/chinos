#include "include/riscv.h"
#include <stdint.h>
void set_mpp(uint64_t mpp) {
    uint64_t mstatus = _read_mstatus();
    mstatus &= ~(3 << 11);

    if (mpp != 0)
        mstatus |= mpp;
    _write_mstatus(mstatus);
}