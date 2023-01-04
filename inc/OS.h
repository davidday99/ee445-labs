// OS.h
// Runs on TM4C123
// OS functions 
// David Day 
// Jan 3, 2022 
#ifndef __OS_H__
#define __OS_H__

#include <stdint.h>


// *************** OS_InitSysTime ********************
// Initialize microsecond precision system timer.
// Inputs: none
// Outputs: none
void OS_InitSysTime(void);

// *************** OS_ClearPeriodicTimer ********************
// Clear system time. 
// Inputs: none
// Outputs: none
void OS_ClearPeriodicTime(void);

// *************** OS_ReadPeriodicTime ********************
// Return system time.
// Inputs: none
// Outputs: none
uint32_t OS_ReadPeriodicTime(void);

// *************** OS_AddPeriodicThread ********************
// Create a thread that periodically performs a task.
// Inputs: task - function to execute
//         period - period at which to run task
//         priority - priority of the task
// Outputs: 1 - task successfully created
//          0 - task creation failed
int OS_AddPeriodicThread(void (*task)(void), uint32_t period, uint32_t priority);

#endif /* __OS_H__ */

