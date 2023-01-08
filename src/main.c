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
#include "sema.h"
#include "DebugTools.h"

char *Input;

int main(void){
    PLL_Init(Bus80MHz);
    Interpreter_Init();
    ST7735_InitR(INITR_GREENTAB);
    OS_InitSysTime();
    ST7735_Message(0, 0, "Hello, world!", 0);
    DebugTools_Init();
    ST7735_Message(1, 1, "System Time: ", OS_ReadPeriodicTime());

    while(1){
        Input = Interpreter_Input("> ");
        Interpreter_Output(Input);
    }
}

