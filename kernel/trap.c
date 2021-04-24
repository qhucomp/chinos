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
void _start_trap(regs reg) {
    int64_t mcause = _read_mcause();
    if (mcause >= 0) {
        switch(mcause) {
            case CAUSE_USER_ECALL:
            case CAUSE_SUPERVISOR_ECALL:
            case CAUSE_HYPERVISOR_ECALL:
                reg.x10 = handle_ecall(reg.x17,&reg);
                break;
            case CAUSE_MACHINE_ECALL: {
                printk("ECALL\n");
                break;
                /*kernel_thread(idle);
                _write_mepc(idle);*/
                return;
            }
            default:
                break;
        }
    } else {
        //printk("timer\n");
        handle_irq_m_ext();
        set_mpp(MPP_SUPERVISOR);
        _write_mepc(_read_mepc());
        return;
        /*handle_irq_m_timer();
        switch(mcause & CAUSE_MACHINE_IRQ_REASON_MASK) {
            case 7: {
                printk("timer!\n");
                handle_irq_m_timer();
                _write_mepc(_read_mepc());
                return;
            }
        }*/
    }
    set_mpp(MPP_SUPERVISOR);
    _write_mepc(_read_mepc() + 4);

}