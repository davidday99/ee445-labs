// System.c
// Runs on TM4C123
// System-related utilities.
// David Day 
// Jan 3, 2022 
#include <stdint.h>
#include "System.h"

uint32_t __SysClockFreq;

uint32_t System_SysClockFreq(void) {
    return  __SysClockFreq;
}

