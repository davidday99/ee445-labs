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

void DebugTools_Init(void);

#endif /* __DEBUGTOOLS_H__ */

