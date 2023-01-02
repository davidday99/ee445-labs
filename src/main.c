// UARTIntsTestMain.c
// Runs on LM4F120/TM4C123
// Tests the UART0 to implement bidirectional data transfer to and from a
// computer running HyperTerminal.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// September 12, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Program 5.11 Section 5.6, Program 3.10

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "UART.h"
#include "ADC.h"
#include "ST7735.h"

#define BUFSIZE 256

static char InputBuf[BUFSIZE];

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void OutCRLF(void){
    UART_OutChar(CR);
    UART_OutChar(LF);
}

void input(char *prompt) {
    UART_OutString(prompt);
    UART_InString(InputBuf, BUFSIZE);
    OutCRLF();
}

//debug code
int main(void){
    PLL_Init(Bus50MHz);       // set system clock to 50 MHz
    UART_Init();              // initialize UART
    ST7735_InitR(INITR_GREENTAB);
    UART_OutString("Hello, world!\n");
    // ST7735_OutString("Hello, world!\n");
    OutCRLF();



    ADC_Open(CHANNEL11); 

    uint16_t x;
    while(1){
        input("> ");
        UART_OutString(InputBuf); 
        x = ADC_In();
        UART_OutUDec(x);
        OutCRLF();
    }
}

