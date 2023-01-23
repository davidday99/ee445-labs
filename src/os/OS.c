// OS.c
// Runs on TM4C123
// OS functions 
// David Day 
// Jan 3, 2022 

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "OS.h"
#include "DebugTools.h"
#include "thread.h"
#include "thread_stack.h"
#include "sched.h"
#include "semaphore.h"

void CPU_OS_Trap(void);
void CPU_OS_EnterTaskMode(void);
void DisableInterrupts(void);

#define TIMER_RELOAD 0xFFFFFFFF      // max value 32-bit register can hold 
#define TIMER_PRESCALE 80            // scale timer down to 1 usec precision at 80 MHz
                    
Tcb_t *RunList;

void idle(void) {
    while (1) {
        OS_Suspend();
    }
}

void OS_InitSysTime(void) {
    SYSCTL_RCGCWTIMER_R |= SYSCTL_RCGCWTIMER_R0;
    int delay = 0;
    while (delay++ < 2);
    WTIMER0_CTL_R &= ~1;  // disable timer
    WTIMER0_CFG_R = (WTIMER0_CFG_R & ~7) | 4;  // enable 32-bit mode so that we can use a prescaler
    WTIMER0_TAMR_R = 2;  // count down, periodic mode
    WTIMER0_TAILR_R = TIMER_RELOAD;
    WTIMER0_TAPR_R = TIMER_PRESCALE;
    WTIMER0_IMR_R = 0;
    WTIMER0_CTL_R |= 1;  // begin counting
}

void OS_ClearPeriodicTime(void) {
    WTIMER0_TAPR_R = TIMER_PRESCALE;
    WTIMER0_TAV_R = TIMER_RELOAD;
}

uint32_t OS_ReadPeriodicTime(void) {
    return TIMER_RELOAD - WTIMER0_TAR_R;  // subtract to get elapsed time
}


void OS_InitSemaphore(semaphore_t *semaPt, long value) {
    semaPt->count = value;
}   

void OS_Wait(semaphore_t *semaPt) {
    while (sema_down(&semaPt->count) == 0) {
        OS_Suspend();
    }
}

void OS_Signal(semaphore_t *semaPt) {
    sema_up(&semaPt->count);
}

void OS_bWait(semaphore_t *semaPt) {
    while (sema_down(&semaPt->count) == 0) {
        OS_Suspend();
    }
}

void OS_bSignal(semaphore_t *semaPt) {
    sema_up(&semaPt->count);
}

int OS_AddThread(void(*task)(void), 
        unsigned long stackSize, unsigned long priority) {
    Tcb_Stack_t *t = Thread_Create();
    Tcb_t *tcb = &t->tcb;
    tcb->stk_sz = stackSize;
    tcb->priority = priority;
    tcb->sp = (uint32_t) Thread_StackInit(t, task, idle);
    tcb->exec = task;
    Sched_AddThread(tcb);
    return 1;
}

unsigned long OS_Id(void) {
    Tcb_t *t = Sched_GetCurrentThread();
    if (t != 0) {
        return t->id;
    }
    return 0;
}

void OS_Kill(void) {
    Tcb_t *t = Sched_GetCurrentThread();
    Sched_RemoveThread(t);
    Thread_Destroy(t);
}

void OS_Suspend(void) {
    CPU_OS_Trap(); 
}

void OS_Launch(unsigned long theTimeSlice) {
    DisableInterrupts();
    Sched_ScheduleNextThread();
    CPU_OS_EnterTaskMode();
}

