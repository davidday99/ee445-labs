// ADC.h
// Runs on TM4C123
// Collect one or multiple samples from desired ADC channel.
// David Day 
// December 30, 2022 
#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>
#include "tm4c123gh6pm.h"

enum ADC_Channel {
    CHANNEL0,
    CHANNEL1,
    CHANNEL2,
    CHANNEL3,
    CHANNEL4,
    CHANNEL5,
    CHANNEL6,
    CHANNEL7,
    CHANNEL8,
    CHANNEL9,
    CHANNEL10,
    CHANNEL11,
};

int ADC_Open(uint32_t channelNum);
uint16_t ADC_In(void);
int ADC_Collect(uint32_t channelNum, uint32_t fs,
        uint16_t buffer[], uint32_t numberOfSamples);
int ADC_Status(void);

#endif /* __ADC_H__ */

