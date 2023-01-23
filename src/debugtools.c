// DebugTools.c
// Runs on TM4C123
// Utility functions helpful for debugging. 
// David Day 
// Jan 3, 2022 

#include "debugTools.h"
#include "tm4c123gh6pm.h"

static void PortE_Init(void);

void DebugTools_Init(void) {
    SYSCTL_RCGCGPIO_R |= 2;
    GPIO_PORTB_DEN_R |= 8;
    GPIO_PORTB_DIR_R |= 8;
    PortE_Init();
}

static void PortE_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x10;       // activate port E
  while((SYSCTL_RCGCGPIO_R&0x10)==0){};      
  GPIO_PORTE_DIR_R |= 0x0F;    // make PE3-0 output heartbeats
  GPIO_PORTE_AFSEL_R &= ~0x0F;   // disable alt funct on PE3-0
  GPIO_PORTE_DEN_R |= 0x0F;     // enable digital I/O on PE3-0
  GPIO_PORTE_PCTL_R = ~0x0000FFFF;
  GPIO_PORTE_AMSEL_R &= ~0x0F;;      // disable analog functionality on PF
}

