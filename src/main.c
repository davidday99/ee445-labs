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
#include "debugTools.h"
#include "thread.h"

unsigned long Count1;
unsigned long Count2;
unsigned long Count3;
unsigned long NumCreated;

void Thread1(void){
  Count1 = 0;          
  for(;;){
    DEBUGPIN_PE0 ^= 0x01;       // heartbeat
    // ST7735_Message(0, 0, "Thread1:", Count1);
    Count1++;
    // OS_Suspend();      // cooperative multitasking
  }
}

void Thread2(void){
  Count2 = 0;          
  for(;;){
    DEBUGPIN_PE1 ^= 0x02;       // heartbeat
    // ST7735_Message(0, 1, "Thread2:", Count2);
    Count2++;
    // OS_Suspend();      // cooperative multitasking
  }
}

void Thread3(void){
  Count3 = 0;          
  for(;;){
    DEBUGPIN_PE2 ^= 0x04;       // heartbeat
    // ST7735_Message(0, 2, "Thread3:", Count3);
    Count3++;
    // OS_Suspend();      // cooperative multitasking
  }
}

int main(void){
    PLL_Init(Bus80MHz);
    Interpreter_Init();
    ST7735_InitR(INITR_GREENTAB);
    OS_InitSysTime();
    DebugTools_Init();
    
    NumCreated = 0;

    NumCreated += OS_AddThread(Thread1, 100, 1);
    NumCreated += OS_AddThread(Thread2, 100, 1);
    NumCreated += OS_AddThread(Thread3, 100, 1);

    OS_Launch(TIME_2MS);
}

