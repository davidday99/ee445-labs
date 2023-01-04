// System.h
// Runs on TM4C123
// System-related utilities.
// David Day 
// Jan 3, 2022 
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

// *************** System_SysClockFreq ********************
// Get the current system clock frequency. 
// Inputs: none 
// Outputs: none
uint32_t System_SysClockFreq(void);

#endif /* __SYSTEM_H__ */

