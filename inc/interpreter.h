// interpreter.h 
// Runs on LM4F120/TM4C123
// Interface for accepting user input and returning output back
// to the user.
// David Day
// Jan 2, 2023
#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#define INTERPRETER_BUFSIZE 256

// ************** Interpreter_Init ************************
// Initializing the interpreter. 
// Input: none 
// Output: none
// ********************************************************
void Interpreter_Init(void);

// ************** Interpreter_Input ************************
// Receive a user inputted string of not more than
// INTERPRETER_BUFSIZE characters.
// Input: prompt - NULL-terminated ASCII string to display to user. 
// Output: const NULL-terminated string
// ********************************************************
const char *Interpreter_Input(char *prompt);

// ************** Interpreter_Output ************************
// Return output to the user.
// Input: string - NULL-terminated ASCII string. 
// Output: none
// ********************************************************
void Interpreter_Output(char *string);

#endif /* __INTERPRETER_H__ */

