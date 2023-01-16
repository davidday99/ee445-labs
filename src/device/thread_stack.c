#include <stdint.h>
#include "thread_stack.h"
#include "thread.h"

uint32_t *Thread_StackInit(Tcb_Stack_t *t,
                           void (*task)(void)) {
    uint32_t *stackptr;
    stackptr = (uint32_t *) &t->stack[THREAD_MAX_STACK_SIZE];
     
    stackptr = (uint32_t *) ((unsigned long) stackptr & 0xFFFFFFF8UL);
    
    *(--stackptr) = 0x01000000;  // xPSR, thumb mode set
    *(--stackptr) = (unsigned long) task;  // entry point
    *(--stackptr) = 0;  // no task return at R14 (LR)
    *(--stackptr) = 0x12121212UL;  // R12
    *(--stackptr) = 0x03030303UL;  // R0-R3
    *(--stackptr) = 0x02020202UL;
    *(--stackptr) = 0x01010101UL;
    *(--stackptr) = 0;
    *(--stackptr) = 0xFFFFFFFDUL;  // exec return with FP disabled
    *(--stackptr) = 0x11111111UL;  // R4-R11
    *(--stackptr) = 0x10101010UL;
    *(--stackptr) = 0x09090909UL;
    *(--stackptr) = 0x08080808UL;
    *(--stackptr) = 0x07070707UL;
    *(--stackptr) = 0x06060606UL;
    *(--stackptr) = 0x05050505UL;
    *(--stackptr) = 0x04040404UL;

    return stackptr;
}
