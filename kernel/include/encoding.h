#ifndef ENCODING_H
#define ENCODING_H

#define MSTATUS_UIE 0x00000001U
#define MSTATUS_SIE 0x00000002U
#define MSTATUS_HIE 0x00000004U
#define MSTATUS_MIE 0x00000008U
#define MSTATUS_UPIE 0x00000010U
#define MSTATUS_SPIE 0x00000020U
#define MSTATUS_HPIE 0x00000040U
#define MSTATUS_MPIE 0x00000080U
#define MSTATUS_SPP 0x00000100U
#define MSTATUS_HPP 0x00000600U
#define MSTATUS_MPP 0x00001800U
#define MSTATUS_FS 0x00006000U
#define MSTATUS_XS 0x00018000U
#define MSTATUS_MPRV 0x00020000U
#define MSTATUS_PUM 0x00040000U
#define MSTATUS_MXR 0x00080000U
#define MSTATUS_VM 0x1F000000U
#define MSTATUS32_SD 0x80000000U
#define MSTATUS64_SD 0x8000000000000000U

#define SSTATUS_UIE 0x00000001U
#define SSTATUS_SIE 0x00000002U
#define SSTATUS_UPIE 0x00000010U
#define SSTATUS_SPIE 0x00000020U
#define SSTATUS_SPP 0x00000100U
#define SSTATUS_FS 0x00006000U
#define SSTATUS_XS 0x00018000U
#define SSTATUS_PUM 0x00040000U
#define SSTATUS32_SD 0x80000000U
#define SSTATUS64_SD 0x8000000000000000U

#define DCSR_XDEBUGVER (3U << 30)
#define DCSR_NDRESET (1U << 29)
#define DCSR_FULLRESET (1U << 28)
#define DCSR_EBREAKM (1U << 15)
#define DCSR_EBREAKH (1U << 14)
#define DCSR_EBREAKS (1U << 13)
#define DCSR_EBREAKU (1U << 12)
#define DCSR_STOPCYCLE (1U << 10)
#define DCSR_STOPTIME (1U << 9)
#define DCSR_CAUSE (7U << 6)
#define DCSR_DEBUGINT (1U << 5)
#define DCSR_HALT (1U << 3)
#define DCSR_STEP (1U << 2)
#define DCSR_PRV (3U << 0)

#define DCSR_CAUSE_NONE 0
#define DCSR_CAUSE_SWBP 1
#define DCSR_CAUSE_HWBP 2
#define DCSR_CAUSE_DEBUGINT 3
#define DCSR_CAUSE_STEP 4
#define DCSR_CAUSE_HALT 5

#define MCONTROL_SELECT (1U << 19)
#define MCONTROL_TIMING (1U << 18)
#define MCONTROL_ACTION (0x3fU << 12)
#define MCONTROL_CHAIN (1U << 11)
#define MCONTROL_MATCH (0xfU << 7)
#define MCONTROL_M (1U << 6)
#define MCONTROL_H (1U << 5)
#define MCONTROL_S (1U << 4)
#define MCONTROL_U (1U << 3)
#define MCONTROL_EXECUTE (1U << 2)
#define MCONTROL_STORE (1U << 1)
#define MCONTROL_LOAD (1U << 0)

#define MCONTROL_TYPE_NONE 0
#define MCONTROL_TYPE_MATCH 2

#define MCONTROL_ACTION_DEBUG_EXCEPTION 0
#define MCONTROL_ACTION_DEBUG_MODE 1
#define MCONTROL_ACTION_TRACE_START 2
#define MCONTROL_ACTION_TRACE_STOP 3
#define MCONTROL_ACTION_TRACE_EMIT 4

#define MCONTROL_MATCH_EQUAL 0
#define MCONTROL_MATCH_NAPOT 1
#define MCONTROL_MATCH_GE 2
#define MCONTROL_MATCH_LT 3
#define MCONTROL_MATCH_MASK_LOW 4
#define MCONTROL_MATCH_MASK_HIGH 5

#define MIP_SSIP (1U << IRQ_S_SOFT)
#define MIP_HSIP (1U << IRQ_H_SOFT)
#define MIP_MSIP (1U << IRQ_M_SOFT)
#define MIP_STIP (1U << IRQ_S_TIMER)
#define MIP_HTIP (1U << IRQ_H_TIMER)
#define MIP_MTIP (1U << IRQ_M_TIMER)
#define MIP_SEIP (1U << IRQ_S_EXT)
#define MIP_HEIP (1U << IRQ_H_EXT)
#define MIP_MEIP (1U << IRQ_M_EXT)

#define SIP_SSIP MIP_SSIP
#define SIP_STIP MIP_STIP

#define PRV_U 0
#define PRV_S 1
#define PRV_H 2
#define PRV_M 3

#define VM_MBARE 0
#define VM_MBB 1
#define VM_MBBID 2
#define VM_SV32 8
#define VM_SV39 9
#define VM_SV48 10

#define IRQ_S_SOFT 1
#define IRQ_H_SOFT 2
#define IRQ_M_SOFT 3
#define IRQ_S_TIMER 5
#define IRQ_H_TIMER 6
#define IRQ_M_TIMER 7
#define IRQ_S_EXT 9
#define IRQ_H_EXT 10
#define IRQ_M_EXT 11
#define IRQ_COP 12
#define IRQ_HOST 13

#define DEFAULT_RSTVEC 0x00001000U
#define DEFAULT_NMIVEC 0x00001004U
#define DEFAULT_MTVEC 0x00001010U
#define CONFIG_STRING_ADDR 0x0000100CU
#define EXT_IO_BASE 0x40000000U
#define DRAM_BASE 0x80000000U

/* page table entry (PTE) fields */
#define PTE_V 0x001U    /* Valid */
#define PTE_R 0x002U    /* Read */
#define PTE_W 0x004U    /* Write */
#define PTE_X 0x008U    /* Execute */
#define PTE_U 0x010U    /* User */
#define PTE_G 0x020U    /* Global */
#define PTE_A 0x040U    /* Accessed */
#define PTE_D 0x080U    /* Dirty */
#define PTE_SOFT 0x300U /* Reserved for Software */

#define PTE_PPN_SHIFT 10

#define MCONTROL_TYPE(xlen) (0xfULL << ((xlen)-4))
#define MCONTROL_DMODE(xlen) (1ULL << ((xlen)-5))
#define MCONTROL_MASKMAX(xlen) (0x3fULL << ((xlen)-11))

#define PTE_TABLE(PTE) (((PTE) & (PTE_V | PTE_R | PTE_W | PTE_X)) == PTE_V)

#endif