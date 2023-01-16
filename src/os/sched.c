#include "sched.h"
#include "thread.h"

Tcb_t *RunningThreadList;
Tcb_t *CurrentThread;

Tcb_t *Sched_GetCurrentThread(void) {
    return CurrentThread;
}

int Sched_AddThread(Tcb_t *t) {
    t->state = RUNNING;
    if (RunningThreadList == 0) {
        RunningThreadList = t;
        RunningThreadList->prev = RunningThreadList;
        RunningThreadList->next = RunningThreadList;
    } else {
        Tcb_t *next = RunningThreadList;

        while (next->next != RunningThreadList && t->priority >= next->priority) {
            next = next->next;
        }

        t->next = next->next;

        /* If inserting at the end of the list, point head->prev to new element */
        if (next->next == RunningThreadList) { 
            RunningThreadList->prev = t;
        }

        next->next = t;
        t->prev = next;
    }
    return 1;
} 

int Sched_RemoveThread(Tcb_t *t) {
    Tcb_t *next = RunningThreadList;

    while (next != t) {
        if (next == 0) {
            return 0;
        }
        next = next->next;
    }

    if (next->next == next) {
        RunningThreadList = 0;
    } else {
        t->prev->next = t->next;
        t->next->prev = t->prev;
    }

    /* If removing head, update head to next in list */
    if (t == RunningThreadList) {
        RunningThreadList = t->next;
    }

    /* If removing the current thread, update accordingly */
    if (t == CurrentThread) {
        CurrentThread = 0;
    }

    t->state = IDLE;
    return 1;
}

