// interpreter.c
// Runs on LM4F120/TM4C123
// Interface for accepting user input and returning output back
// to the user.
// David Day
// Jan 2, 2023
#include "interpreter.h"
#include "UART.h"
#include "ADC.h"
#include "ST7735.h"
#include "stdlib.h"
#include "string.h"
#include "OS.h"

#define MAX_ARG_COUNT 10

typedef struct _Command {
    int argc;
    const char **cmd;
    void (*exec)(int, char **);
} Command;

void exec_systime(int argc, char *argv[]);
void exec_print(int argc, char *argv[]);
void exec_adc_in(int argc, char *argv[]);
void exec_adc_collect(int argc, char *argv[]);
void exec_adc_status(int argc, char *argv[]);
void usage(char *s);

static char InputBuf[INTERPRETER_BUFSIZE];

static char *ArgBuf[MAX_ARG_COUNT];

int ParseArgs(char *string);

static void OutCRLF(void);

const Command Commands[] = {
    {1, (const char * []){"systime"}, exec_systime},
    {5, (const char * []){"print", "%d", "%d", "%d", "%s"}, exec_print},
    {2, (const char * []){"adc_in", "%d"}, exec_adc_in},
    {5, (const char * []){"adc_collect", "%d", "%d", "%d"}, exec_adc_collect},
    {1, (const char * []){"adc_status"}, exec_adc_status}
};

void Interpreter_Init(void) {
    UART_Init();
}

char *Interpreter_Input(char *prompt) {
    UART_OutString(prompt);
    UART_InString(InputBuf, INTERPRETER_BUFSIZE);
    OutCRLF();
    return InputBuf;
}

void Interpreter_Output(char *string) {
    int argc = ParseArgs(string);
    const char *cmd = ArgBuf[0];

    for (unsigned long i = 0; i < (sizeof(Commands) / sizeof(Command)); i++) {
        if (strcmp(cmd, Commands[i].cmd[0]) == 0) {
            Commands[i].exec(argc, ArgBuf); 
            OutCRLF();
        }
    }
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

void exec_systime(int argc, char *argv[]) {
    UART_OutUDec(OS_ReadPeriodicTime());
}

void exec_print(int argc, char *argv[]) {
    int device, line, value;
    if (argv[1] != 0 && argv[2] != 0 && argv[3] != 0 && argv[4] != 0) {
        device = atoi(argv[1]);
        line = atoi(argv[2]);
        value = atoi(argv[3]);
        ST7735_Message(device, line, argv[4], value);
    } else {
        usage("print [device] [line] [value] [string]");
    }
}

void exec_adc_in(int argc, char *argv[]) {
    if (argv[1] != 0 && ADC_Open(atoi(argv[1]))) {
        UART_OutUDec(ADC_In());
    } else {
        usage("adc_in [channel]");
    }    
}

void exec_adc_collect(int argc, char *argv[]) {
    int channelNum, fs, numberofSamples;
    static uint16_t buf[1000];
    if (argv[1] == 0 || argv[2] == 0 || argv[3] == 0) {
        usage("adc_collect [channel] [frequency] [sample count]");
        return;
    }
    channelNum = atoi(argv[1]);
    fs = atoi(argv[2]);
    numberofSamples = atoi(argv[3]);
    ADC_Collect(channelNum, fs, buf, numberofSamples);
}


void exec_adc_status(int argc, char *argv[]) {
    UART_OutUDec(ADC_Status());
}

void usage(char *s) {
    UART_OutString(s);
    OutCRLF();
}


//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
static void OutCRLF(void){
    UART_OutChar(CR);
    UART_OutChar(LF);
}
