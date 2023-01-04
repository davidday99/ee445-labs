// OS.h
// Runs on TM4C123
// OS functions 
// David Day 
// Jan 3, 2022 
#ifndef __OS_H__
#define __OS_H__

#include <stdint.h>

void OS_InitSysTime(void);

void OS_ClearPeriodicTime(void);

uint32_t OS_ReadPeriodicTime(void);

int OS_AddPeriodicThread(void (*task)(void), uint32_t period, uint32_t priority);

#endif /* __OS_H__ */

