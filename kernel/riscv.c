#include "include/riscv.h"
#include <stdint.h>
void set_mpp(uint64_t mpp) {
    uint64_t _mstatus = _read_mstatus();
    _mstatus &= ~(3 << 11);

    if (mpp != 0)
        _mstatus |= mpp;
    _write_mstatus(_mstatus);
}