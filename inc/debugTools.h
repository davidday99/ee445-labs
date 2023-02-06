// DebugTools.h
// Runs on TM4C123
// Utility functions helpful for debugging. 
// David Day 
// Jan 3, 2022 
#ifndef __DEBUGTOOLS_H__
#define __DEBUGTOOLS_H__

#include "tm4c123gh6pm.h"

#define DEBUGPIN_HIGH()     (GPIO_PORTB_DATA_R |= 8)
#define DEBUGPIN_LOW()      (GPIO_PORTB_DATA_R &= ~8)

#define DEBUGPIN_PB3  (*((volatile unsigned long *)0x40005020))

#define DEBUGPIN_PE0  (*((volatile unsigned long *)0x40024004))
#define DEBUGPIN_PE1  (*((volatile unsigned long *)0x40024008))
#define DEBUGPIN_PE2  (*((volatile unsigned long *)0x40024010))
#define DEBUGPIN_PE3  (*((volatile unsigned long *)0x40024020))

void DebugTools_Init(void);

#endif /* __DEBUGTOOLS_H__ */

