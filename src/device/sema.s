.syntax unified
.p2align 2

.global sema_down
.type sema_down, %function
sema_down:
    .fnstart
    LDREXB R1, [R0]
    CMP R1, #0
    BGT store
    DMB
    MOV R0, #0
    BX LR
store:
    SUB R1, #1
    STREXB R2, R1, [R0]
    CMP R2, #0
    BNE sema_down
    DMB
    MOV R0, #1
    BX LR
    .fnend

.global sema_up
.type sema_up, %function
sema_up:
    .fnstart
    LDREXB R1, [R0]
    ADD R1, #1
    STREXB R2, R1, [R0]   
    CMP R2, #0
    BNE sema_up
    DMB
    BX LR
    .fnend

