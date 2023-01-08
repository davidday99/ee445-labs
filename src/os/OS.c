// OS.c
// Runs on TM4C123
// OS functions 
// David Day 
// Jan 3, 2022 

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "OS.h"
#include "DebugTools.h"

#define TIMER_RELOAD 0xFFFFFFFF      // max value 32-bit register can hold 
#define TIMER_PRESCALE 80            // scale timer down to 1 usec precision at 80 MHz

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

int OS_AddPeriodicThread(void (*task)(void), uint32_t period, uint32_t priority) {
    return 1;
}

