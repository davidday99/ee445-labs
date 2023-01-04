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

#define ADC_SAMPLE_COUNT    1000
#define ADC_SAMPLE_FREQ     2000

uint16_t SampleBuf[ADC_SAMPLE_COUNT];

//debug code
int main(void){
    PLL_Init(Bus80MHz);       // set system clock to 80 MHz
    Interpreter_Init();
    ST7735_InitR(INITR_GREENTAB);
    OS_InitSysTime();
    ST7735_Message(0, 0, "Hello, world!", 0);
    
    ADC_Open(ADC_CHANNEL11);
    uint16_t s = ADC_In();

    ST7735_Message(0, 1, "Sampled ", s);

    ADC_Collect(ADC_CHANNEL11, ADC_SAMPLE_COUNT, SampleBuf, ADC_SAMPLE_COUNT);

    while (ADC_Status() != 0)
        ;

    ST7735_Message(1, 0, "Sampling Done!", 0);

    ST7735_Message(1, 1, "System Time: ", OS_ReadPeriodicTime());

    while(1){
        Interpreter_Input("> ");
    }
}

