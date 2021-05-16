#include "include/trap.h"
#include "include/uart.h"
#include "include/riscv.h"
#include "include/syscalls.h"
#include "include/printk.h"
#include "include/kernel_thread.h"
#include "include/idle.h"
#include "include/timer.h"
#include "include/plic.h"
#include "include/thread_test.h"
#include "include/sysctl.h"
void _start_trap(regs *reg) {
    int64_t mcause = read_csr(mcause);
    printk("trap %p pid=%d\n",mcause,current->pid);
    if (mcause >= 0) {
        switch(mcause) {
            case CAUSE_USER_ECALL:
            case CAUSE_SUPERVISOR_ECALL:
            case CAUSE_HYPERVISOR_ECALL:
            case CAUSE_MACHINE_ECALL:
                if (!reg->x17) {
                    write_csr(mepc,current->epc);
                    reg->x12 = 0x80100000;
                    current->sp = 0x80100000;
                    current->flag &= ~TASK_FLAG_NO_RUN;
                    set_mpp(MPP_MACHINE);
                    return;
                }
                current->epc = read_csr(mepc);
                current->sp =  read_csr(mscratch);
                current->task_reg = *reg;
                reg->x10 = handle_ecall(reg->x17,reg);
                write_csr(mepc,current->epc + 4);
                break;
            default:
                printk("mepc:%p\n",read_csr(mepc));
                printk("instruction:%p\n",*((char *)read_csr(mepc)));
                write_csr(mepc,0x803000b2);
                break;
        }
    } else {
        //在切换进程前保存pc和寄存器
        // current->epc = read_csr(mepc);
        // current->sp =  read_csr(mscratch);
        // current->task_reg = *reg;
        // handle_irq_m_ext();

        // //如果是第一次运行则不需要恢复寄存器
        // if (!(current->flag & TASK_FLAG_NO_RUN)){
        //     current->task_reg.x2 = reg->x2;
        //     *reg = current->task_reg;
        // }

        // if (current->flag & TASK_FLAG_NO_RUN)
        //     current->flag &= ~TASK_FLAG_NO_RUN;
        
        // printk("switch epc:%p\n",current->epc);
        // write_csr(mepc,current->epc);
        printk("mepc=%p\n",read_csr(mepc));
        write_csr(mepc,read_csr(mepc));
        set_mpp(MPP_MACHINE);
        // reg->x12 = current->sp;
        return;
    }
    reg->x12 = current->sp;
    set_mpp(MPP_MACHINE);
}