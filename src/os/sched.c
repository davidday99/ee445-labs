#include "sched.h"
#include "thread.h"

static Tcb_t *RunningThreadList;
static Tcb_t *CurrentThread;

Tcb_t *Sched_GetCurrentThread(void) {
    return CurrentThread;
}

int Sched_AddThread(Tcb_t *t) {
    if (RunningThreadList == 0) {
        RunningThreadList = t;
    } else {
        Tcb_t *next = RunningThreadList;
        while (next->next != 0) {
            next = next->next;
        }
        next->next = t;
        t->prev = next;
        t->next = 0;
    }
    return 1;
} 

int Sched_RemoveThread(Tcb_t *t) {
    Tcb_t *next = RunningThreadList;
    while (next != t) {
        if (next == 0) {
            return 0;
        }
    }
    if (next->prev == 0 && next->next == 0) {
        RunningThreadList = 0;
    }
    if (next->prev == 0) {
        RunningThreadList = next->next;
        RunningThreadList->prev = 0;
    }
    next->prev = next->next;
    if (next->next != 0) {
        next->next->prev = next->prev; 
    }
    return 1;
}
