// interpreter.c
// Runs on LM4F120/TM4C123
// Interface for accepting user input and returning output back
// to the user.
// David Day
// Jan 2, 2023
#include "interpreter.h"
#include "UART.h"
#include "commands.h"
#include "string.h"

#define MAX_ARG_COUNT 10

static char InputBuf[INTERPRETER_BUFSIZE];

static char *ArgBuf[MAX_ARG_COUNT];

int ParseArgs(char *string);
void ClearArgs(void);

void Interpreter_Init(void) {
    UART_Init();
}

char *Interpreter_Input(char *prompt) {
    UART_OutString(prompt);
    UART_InString(InputBuf, INTERPRETER_BUFSIZE);
    UART_OutCRLF();
    return InputBuf;
}

void Interpreter_Output(char *string) {
    int argc = ParseArgs(string);
    const char *cmd = ArgBuf[0];

    for (unsigned long i = 0; i < CommandCount; i++) {
        if (strcmp(cmd, Commands[i].cmd[0]) == 0) {
            Commands[i].exec(argc, ArgBuf); 
            UART_OutCRLF();
        }
    }
    ClearArgs();
}

int ParseArgs(char *string) {
    char *c = string; 
    int i = 0;
    int argCount = 0;
    char delim;

    while (*c != '\0' && i < INTERPRETER_BUFSIZE) {

        while (*c == ' ' && i < INTERPRETER_BUFSIZE) {
            c++;
            i++; 
        } 

        // Allow quotes to be used to pass in an argument containing spaces.
        // All single quotes inside a double quoted string are interpreted 
        // as literals, and vice-versa.
        if (*c == '"' || *c == '\'') {
            delim = *c;
            c++;
        } else {
            delim = ' ';
        }
        
        ArgBuf[argCount++] = c;
         
        while (*c != delim && *c != '\0' && i < INTERPRETER_BUFSIZE) {
            c++;
            i++;
        }

        if (*c == '\0') {
            break;
        } else {
            *c++ = '\0';
            i++;
        }
    } 
    return argCount;
}

void ClearArgs(void) {
    for (unsigned long i = 0; i < sizeof(ArgBuf) / sizeof(ArgBuf[0]); i++) {
        ArgBuf[i] = 0;
    }
}

