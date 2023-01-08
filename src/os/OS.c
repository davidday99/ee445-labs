// OS.c
// Runs on TM4C123
// OS functions 
// David Day 
// Jan 3, 2022 

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "OS.h"
#include "DebugTools.h"

#define SYSTICK_RELOAD 79      // 1 us period for 80 MHz system clock

static uint32_t SysTimeMicroSeconds;

void OS_InitSysTime(void) {
    NVIC_ST_CTRL_R &= 0x7;  // clear SysTick control register
    NVIC_ST_RELOAD_R &= 0x00FFFFFF;
    NVIC_ST_RELOAD_R = SYSTICK_RELOAD;
    NVIC_ST_CTRL_R |= 7;  // enable clock with interrupts, clk src is sys clock
}

void SysTick_Handler(void) {
    SysTimeMicroSeconds++;
}

void OS_ClearPeriodicTime(void) {
    SysTimeMicroSeconds = 0;
}

uint32_t OS_ReadPeriodicTime(void) {
    return SysTimeMicroSeconds;
}

int OS_AddPeriodicThread(void (*task)(void), uint32_t period, uint32_t priority) {
    return 1;
}

