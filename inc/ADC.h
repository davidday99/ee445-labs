// ADC.h
// Runs on TM4C123
// Collect samples from ADC0.
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

// *************** ADC_Open ********************
// Open an ADC channel for performing a 
// software-triggered sample. 
// Inputs: channelNum - ADC channel 0-11 
// Outputs: none
int ADC_Open(uint32_t channelNum);

// *************** ADC_In ********************
// Perform a software triggered sampling 
// on the channel for which the last ADC_Open.
// call was made.
// Inputs: none 
// Outputs: none
uint16_t ADC_In(void);

// *************** ADC_Collect ********************
// Write a timer-triggered collection of ADC
// samples into a buffer. 
// Inputs: channelNum - ADC channel 0-11
//         fs         - frequency of timer trigger
//         buffer     - location to store samples
//         numberOfSamples - size of buffer
//         y2 is the y coordinate of the second point plotted
// Outputs: 1 - sampling initiated
//          0 - bad channel or frequency input
int ADC_Collect(uint32_t channelNum, uint32_t fs,
        uint16_t buffer[], uint32_t numberOfSamples);

// *************** ADC_Status ********************
// Get status of ADC_Collect call.
// Inputs: none 
// Outputs: 1 - ADC_Collect still sampling 
//          0 - ADC_Collect completed
int ADC_Status(void);

#endif /* __ADC_H__ */

