#include "include/trap.h"
#include "include/uart.h"
#include "include/riscv.h"
#include "include/syscalls.h"
#include "include/printk.h"
#include "include/user_thread.h"
#include "include/user_test.h"
void _start_trap(regs reg) {
    uint64_t mcause = _read_mcause();
    //printk("mcause:%d\n",mcause);
    switch(mcause) {
        case CAUSE_USER_ECALL:
        case CAUSE_SUPERVISOR_ECALL:
        case CAUSE_HYPERVISOR_ECALL:
            reg.x10 = handle_ecall(reg.x17,&reg);
            break;
        case CAUSE_MACHINE_ECALL: {
            user_thread *thread = create_thread(user_test);
            reg = *thread->reg;
            set_mpp(MPP_SUPERVISOR);
            _write_mepc(thread->epc);
            return;
        }
        default:
            break;
    }
    set_mpp(MPP_SUPERVISOR);
    _write_mepc(_read_mepc() + 4);

}