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

//debug code
int main(void){
    PLL_Init(Bus50MHz);       // set system clock to 50 MHz
    Interpreter_Init();
    ST7735_InitR(INITR_GREENTAB);

    ST7735_Message(0, 0, "Hello, world!", 0);
    ST7735_Message(1, 0, "Hello, world!", 0);
    ST7735_Message(1, 1, "Hello, world!", 0);
    ST7735_Message(0, 1, "Hello, world!", 0);

    ADC_Open(CHANNEL11); 

    while(1){
        Interpreter_Input("> ");
    }
}

