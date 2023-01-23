.syntax unified 
.p2align 2

.equ NVIC_INT_CTRL,     0xE000ED04
.equ NVIC_SYSPRI14,     0xE000ED22
.equ NVIC_PENDSV_PRI,   0xE0
.equ NVIC_PENDSVSET,    0x10000000
.equ NVIC_PENDSVCLR,    0x08000000

.extern             __stack_ptr
.extern             Sched_CurrentThreadPtr
.extern             Sched_ScheduleNextThread 

@**************************************************
@ CPU_OS_EnterTaskMode 
@ preconditions: - CPU is running in Thread mode 
@                  and using MSP
@               
@ postconditions: - CPU is running current task
@                 - CPU is running in CPU_OS_EnterTaskMode
@                 - PendSV_Handler is set to 
@                   lowest priority 
@
@**************************************************
.global CPU_OS_EnterTaskMode 
.type CPU_OS_EnterTaskMode, %function 
CPU_OS_EnterTaskMode:
    LDR R1, =_stack_ptr        @ reset MSP to starting position
    MSR MSP, R1

    LDR R1, =NVIC_SYSPRI14     @ set PendSV to lowest priority
    LDR R0, =NVIC_PENDSV_PRI
    STR R0, [R1]

    MOVW R0, #:lower16:Sched_CurrentThreadPtr
    MOVT R0, #:upper16:Sched_CurrentThreadPtr
    LDR R1, [R0]
    LDR R2, [R1]    @ R2 is SP of current thread 
    MSR PSP, R2

    MRS R1, CONTROL
    ORR R1, R1, #2      @ switch from MSP to PSP
    BIC R1, R1, #4      @ clear FPCA bit to indicate FPU is not in use 
    MSR CONTROL, R1
    ISB

    LDMFD SP!, {R4-R11}     @ restore r4-r11 and lr from new task stack
    LDMFD SP!, {R0-R3}          @ restore r0-r3, from now on this will be handled by processor
    LDMFD SP!, {R12, LR}        @ restore r12 and lr, same as above
    LDMFD SP!, {R1, R2}         @ load pc and discard PSR
    CPSIE I
    BX R1

@**************************************************
@ CPU_OS_Trap 
@ preconditions: CPU is running in task mode 
@                 
@ postconditions: CPU jumps to PendSV vector
@
@**************************************************
.global CPU_OS_Trap 
.type CPU_OS_Trap, %function 
CPU_OS_Trap:
    LDR R0, =NVIC_INT_CTRL
    LDR R1, =NVIC_PENDSVSET
    STR R1, [R0]
    BX LR

@**************************************************
@ PendSV_Handler 
@
@ This subroutine can be broken down into three
@ steps-
@    1. Save state of current task.
@    2. Perform OS maintainance such as 
@       scheduling the next thread.
@    3. Restore the state of R4-R11 and LR of
@        the new task.
@ On returning from this subroutine, the processor
@ will handle restoring the state of the R0-R3,
@ R12, LR, PC, and xPSR, which will all be 
@ present on the stack. These values are
@ guaranteed to be present on a well-formed stack
@ because either the task is running for the first
@ time, in which case the stack was initialized with
@ these values, or the task is running again after
@ a previous context switch, in which case the
@ processor would have pushed these values onto
@ the stack.
@
@ preconditions: - CPU is executing from 
@                  PendSV vector in Handler Mode
@                 
@ postconditions: - CPU is running next task 
@                   based on scheduler 
@
@**************************************************
.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
    PUSH { R5, R6 }
    MRS R0, PSP     @ R0 contains SP of current task
    STMFD R0!, {R4-R11}
    MOVW R5, #:lower16:Sched_CurrentThreadPtr
    MOVT R5, #:upper16:Sched_CurrentThreadPtr
    LDR R1, [R5]
    STR R0, [R1]    @ R0 is new SP of current task 

    MOV R6, LR
    BL Sched_ScheduleNextThread 
    MOV LR, R6

    LDR R0, [R5]    @ R0 is SP of current stack
    LDR R0, [R0]
    LDMFD R0!, {R4-R11}
    MSR PSP, R0     @ update PSP after popping some values
    DSB
    ISB
    POP { R5, R6 }
    BX LR

