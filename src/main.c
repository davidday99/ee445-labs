// main.c 
// Runs on LM4F120/TM4C123
// Main program loop, waits for input from interpreter. 
// David Day
// Jan 2, 2023 

#include <stdint.h>
#include "PLL.h"
#include "ADC.h"
#include "UART.h"
#include "ST7735.h"
#include "interpreter.h"
#include "OS.h"

//debug code
int main(void){
    PLL_Init(Bus80MHz);       // set system clock to 80 MHz
    Interpreter_Init();
    ST7735_InitR(INITR_GREENTAB);
    // OS_InitSysTime();

    uint16_t buf[1000];

    ADC_Collect(CHANNEL11, 1000, buf, 1000);

    while (ADC_Status() != 0)
        ;
    ST7735_OutString("Done!");

    while(1){
        Interpreter_Input("> ");
        /*UART_OutUDec(OS_ReadPeriodicTime());*/
        /*UART_OutChar(CR);*/
        /*UART_OutChar(LF);*/
    }
}

