#ifndef __TCB_H__
#define __TCB_H__

#include <stdint.h>

#define THREAD_MAX_STACK_SIZE 512
#define THREAD_SP_OFFSET offsetof(struct Tcb, sp)

typedef enum Thread_State {
    SLEEPING,
    RUNNING,
    BLOCKED
} Thread_State_t;

typedef struct Tcb {
    uint32_t sp;
    struct Tcb *prev;
    struct Tcb *next;
    enum Thread_State state;
    uint8_t priority;
} Tcb_t;

typedef struct Tcb_Stack {
    struct Tcb tcb; 
    uint8_t stack[THREAD_MAX_STACK_SIZE];
} Tcb_Stack_t;

Tcb_t *Thread_Create(void);
int Thread_Destroy(Tcb_t *t);

#endif /* __TCB_H__ */

