#ifndef __THREAD_STACK_H__
#define __THREAD_STACK_H__


#include "thread.h"

uint32_t *Thread_StackInit(Tcb_Stack_t *t,
                           void (*task)(void));

#endif /* __THREAD_STACK_H__ */

