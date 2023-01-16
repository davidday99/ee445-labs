.syntax unified 
.p2align 2

.equ NVIC_INT_CTRL,     0xE000ED04
.equ NVIC_SYSPRI14,     0xE000ED22
.equ NVIC_PENDSV_PRI,   0xFF
.equ NVIC_PENDSVSET,    0x10000000

@**************************************************
@ ctx_switch
@ preconditions: CPU is running in task mode 
@ postconditions: CPU is running in handler mode
@                 in PendSV_Handler() 
@
@**************************************************
.global ctx_switch
.type ctx_switch, %function 
ctx_switch:
    LDR R0, =NVIC_INT_CTRL
    LDR R1, =NVIC_PENDSVSET
    STR R1, [R0]
    BX LR
    
