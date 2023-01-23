// main.c 
// Runs on LM4F120/TM4C123
// Main program loop, waits for input from interpreter. 
// David Day
// Jan 2, 2023 

#include <stdint.h>
#include "PLL.h"
#include "ADC.h"
#include "ST7735.h"
#include "interpreter.h"
#include "OS.h"
#include "DebugTools.h"
#include "thread.h"

#define PE0  (*((volatile unsigned long *)0x40024004))
#define PE1  (*((volatile unsigned long *)0x40024008))
#define PE2  (*((volatile unsigned long *)0x40024010))
#define PE3  (*((volatile unsigned long *)0x40024020))

unsigned long Count1;
unsigned long Count2;
unsigned long Count3;
unsigned long NumCreated;

void Thread1(void){
  Count1 = 0;          
  for(;;){
    PE0 ^= 0x01;       // heartbeat
    ST7735_Message(0, 0, "Thread1:", Count1++);
    DebugTools_Init();
    OS_Suspend();      // cooperative multitasking
  }
}

void Thread2(void){
  Count2 = 0;          
  for(;;){
    PE1 ^= 0x02;       // heartbeat
    ST7735_Message(0, 1, "Thread2:", Count2++);
    OS_Suspend();      // cooperative multitasking
  }
}

void Thread3(void){
  Count3 = 0;          
  for(;;){
    PE2 ^= 0x04;       // heartbeat
    ST7735_Message(0, 2, "Thread3:", Count3++);
    OS_Suspend();      // cooperative multitasking
  }
}

void PortE_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x10;       // activate port E
  while((SYSCTL_RCGCGPIO_R&0x10)==0){};      
  GPIO_PORTE_DIR_R |= 0x0F;    // make PE3-0 output heartbeats
  GPIO_PORTE_AFSEL_R &= ~0x0F;   // disable alt funct on PE3-0
  GPIO_PORTE_DEN_R |= 0x0F;     // enable digital I/O on PE3-0
  GPIO_PORTE_PCTL_R = ~0x0000FFFF;
  GPIO_PORTE_AMSEL_R &= ~0x0F;;      // disable analog functionality on PF
}

int main(void){
    PLL_Init(Bus80MHz);
    Interpreter_Init();
    ST7735_InitR(INITR_GREENTAB);
    PortE_Init();
    // OS_InitSysTime();
    DebugTools_Init();
    
    NumCreated = 0;

    NumCreated += OS_AddThread(Thread1, 100, 1);
    NumCreated += OS_AddThread(Thread2, 100, 1);
    NumCreated += OS_AddThread(Thread3, 100, 1);

    OS_Launch(100);
}

