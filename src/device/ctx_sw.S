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
@ ctx_switch
@ preconditions: CPU is running in task mode 
@                 
@ postconditions: CPU jumps to PendSV vector
@
@**************************************************
.global ctx_switch
.type ctx_switch, %function 
ctx_switch:
    LDR R0, =NVIC_INT_CTRL
    LDR R1, =NVIC_PENDSVSET
    STR R1, [R0]
    BX LR


@**************************************************
@ svcall
@ preconditions: CPU is running in task mode 
@                 
@ postconditions: CPU jumps to SVCall vector
@
@**************************************************
.global svcall 
.type svcall, %function 
svcall:
    MOV R5, LR
    SVC #1
    MOV LR, R5
    BX LR

@**************************************************
@ PendSV_Handler 
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
    MRS R0, PSP     @ R0 contains SP of current task
    STMFD R0!, {R4-R11, LR}
    MOVW R4, #:lower16:Sched_CurrentThreadPtr
    MOVT R4, #:upper16:Sched_CurrentThreadPtr
    LDR R1, [R4]
    CMP R1, #0
    BEQ currentnull
    STR R0, [R1]    @ R0 is new SP of current task 
currentnull:
    MOV R5, LR      @ save LR
    BL Sched_ScheduleNextThread 
    MOV LR, R5
    LDR R0, [R4]    @ R0 is SP of next task to run
    LDR R0, [R0]
    LDMFD R0!, {R4-R11, LR}
    MSR PSP, R0
    DSB
    ISB
    BX LR
       
    

@**************************************************
@ enter_task_mode 
@ preconditions: - CPU is running in regular 
@                  startup mode
@               
@ postconditions: - CPU is running current task
@                 - PSP is enabled as the SP
@                 - PendSV_Handler is set to 
@                   lowest priority 
@
@**************************************************
.global enter_task_mode 
.type enter_task_mode, %function 
enter_task_mode:
    LDR R1, =_stack_ptr        @ reset MSP to starting position
    MSR MSP, R1

    LDR R1, =NVIC_SYSPRI14     @ set PendSV to lowest priority
    LDR R0, =NVIC_PENDSV_PRI
    STR R0, [R1]

    LDR R0, =Sched_CurrentThreadPtr
    LDR R1, [R0]
    LDR R1, [R1]
    MSR PSP, R1
    MRS R1, CONTROL
    ORR R1, R1, #2      @ switch from MSP to PSP
    BIC R1, R1, #4      @ clear FPCA bit to indicate FPU is not in use 
    MSR CONTROL, R1
    ISB

    LDMFD SP!, {R4-R11, LR}     @ restore registers of task to run
    LDMFD SP!, {R0-R3}
    LDMFD SP!, {R12, LR}
    LDMFD SP!, {R1, R2}         @ R1 contains entry point, discard LR
    BX R1
 
