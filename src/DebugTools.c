// DebugTools.c
// Runs on TM4C123
// Utility functions helpful for debugging. 
// David Day 
// Jan 3, 2022 

#include "DebugTools.h"
#include "tm4c123gh6pm.h"

void DebugTools_Init(void) {
    SYSCTL_RCGCGPIO_R |= 2;
    GPIO_PORTB_DEN_R |= 8;
    GPIO_PORTB_DIR_R |= 8;
}

