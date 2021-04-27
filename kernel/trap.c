#include "include/trap.h"
#include "include/uart.h"
#include "include/riscv.h"
#include "include/syscalls.h"
#include "include/printk.h"
#include "include/kernel_thread.h"
#include "include/idle.h"
#include "include/timer.h"
#include "include/clint.h"
#include "include/plic.h"
#include "include/thread_test.h"
void _start_trap(regs *reg) {
    int64_t mcause = _read_mcause();

    if (mcause >= 0) {
        switch(mcause) {
            case CAUSE_USER_ECALL:
            case CAUSE_SUPERVISOR_ECALL:
            case CAUSE_HYPERVISOR_ECALL:
                reg->x10 = handle_ecall(reg->x17,reg);
                break;
            case CAUSE_MACHINE_ECALL: {
                set_mpp(MPP_MACHINE);
                _write_mepc(current->epc);
                reg->x11 = current->sp;
                current->flag = 1;
                return;
            }
            default:
                break;
        }
    } else {
        current->epc = read_csr(mepc);
        current->sp =  read_csr(mscratch);
        current->task_reg = *reg;
        handle_irq_m_ext();
        if (current->flag){
            current->task_reg.x2 = reg->x2;
            *reg = current->task_reg;
        } else {
            current->flag = 1;
        }
        write_csr(mepc,current->epc);
        reg->x11 = current->sp;
        return;
    }
    set_mpp(MPP_SUPERVISOR);
    _write_mepc(_read_mepc() + 4);
}