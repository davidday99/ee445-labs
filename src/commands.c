// commands.h
// Runs on LM4F120/TM4C123
// Create commands that call into system functions. 
// David Day
// Jan 7, 2023
#include "commands.h"
#include "UART.h"
#include "ADC.h"
#include "OS.h"

static uint16_t AdcBuf[2000];

/* Declare the function associated with the command here */
void exec_systime(int argc, char *argv[]);
void exec_print(int argc, char *argv[]);
void exec_adc_in(int argc, char *argv[]);
void exec_adc_collect(int argc, char *argv[]);
void exec_adc_status(int argc, char *argv[]);
void exec_adc_samples(int argc, char *argv[]);
void usage(char *s);

/* Add new commands here */
const Command Commands[] = {
    {1, (const char * []){"systime"}, exec_systime},
    {5, (const char * []){"print", "%d", "%d", "%d", "%s"}, exec_print},
    {2, (const char * []){"adc_in", "%d"}, exec_adc_in},
    {5, (const char * []){"adc_collect", "%d", "%d", "%d"}, exec_adc_collect},
    {1, (const char * []){"adc_status"}, exec_adc_status},
    {1, (const char * []){"adc_samples"}, exec_adc_samples},
};

const unsigned long CommandCount = sizeof(Commands) / sizeof(Command);

void usage(char *s) {
    UART_OutString(s);
    UART_OutCRLF();
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
    if (argv[1] == 0 || argv[2] == 0 || argv[3] == 0) {
        usage("adc_collect [channel] [frequency] [sample count]");
        return;
    }
    channelNum = atoi(argv[1]);
    fs = atoi(argv[2]);
    numberofSamples = atoi(argv[3]);
    ADC_Collect(channelNum, fs, AdcBuf, numberofSamples);
}


void exec_adc_status(int argc, char *argv[]) {
    UART_OutUDec(ADC_Status());
}

void exec_adc_samples(int argc, char *argv[]) {
    if (ADC_Status() == 1) {
        UART_OutString("Conversion in progress...");
    } else {
        for (unsigned long i = 0; i < sizeof(AdcBuf) / sizeof(AdcBuf[0]); i++) {
            UART_OutUDec(AdcBuf[i]);
            UART_OutCRLF();
        }
    } 
}

