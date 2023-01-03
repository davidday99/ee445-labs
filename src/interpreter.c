// interpreter.c
// Runs on LM4F120/TM4C123
// Interface for accepting user input and returning output back
// to the user.
// David Day
// Jan 2, 2023
#include "interpreter.h"
#include "UART.h"

static char InputBuf[INTERPRETER_BUFSIZE];

static void OutCRLF(void);

void Interpreter_Init(void) {
    UART_Init();
}

const char *Interpreter_Input(char *prompt) {
    UART_OutString(prompt);
    UART_InString(InputBuf, INTERPRETER_BUFSIZE);
    OutCRLF();
    return InputBuf;
}

void Interpreter_Output(char *string) {
    return;
}

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
static void OutCRLF(void){
    UART_OutChar(CR);
    UART_OutChar(LF);
}
