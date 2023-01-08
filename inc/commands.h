// commands.h
// Runs on LM4F120/TM4C123
// Create commands that call into system functions. 
// David Day
// Jan 7, 2023
#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <stdint.h>

typedef struct _Command {
    int argc;
    const char **cmd;
    void (*exec)(int, char **);
} Command;

extern const Command Commands[];
extern const unsigned long CommandCount;

#endif /* __COMMANDS_H__ */

