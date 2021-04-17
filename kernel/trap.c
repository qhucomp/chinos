#include "include/trap.h"
#include "include/uart.h"
#include "include/riscv.h"
#include "include/syscalls.h"
void _start_trap(regs reg) {
    uint64_t mcause = read_mcause();
    switch(mcause) {
        case CAUSE_USER_ECALL:
        case CAUSE_SUPERVISOR_ECALL:
        case CAUSE_HYPERVISOR_ECALL:
        case CAUSE_MACHINE_ECALL:
            reg.x10 = handle_ecall(reg.x17,&reg);
            break;
    }
    set_mpp(MPP_SUPERVISOR);
    write_mepc(read_mepc() + 4);

}