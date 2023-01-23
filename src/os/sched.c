#include "sched.h"
#include "thread.h"
#include "debugTools.h"

Tcb_t *RunningThreadList;
Tcb_t *Sched_CurrentThreadPtr;

Tcb_t *Sched_GetCurrentThread(void) {
    return Sched_CurrentThreadPtr;
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

    /* If only one element in list, set list head to null */
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
    if (t == Sched_CurrentThreadPtr) {
        Sched_CurrentThreadPtr = 0;
    }

    t->state = IDLE;
    return 1;
}

void Sched_ScheduleNextThread(void) {
    static Tcb_t *next;
    DEBUGPIN_PE3 ^= 0x08;
    if (next == 0) {
        next = RunningThreadList;
    } else {
        next = next->next;
    }
    Sched_CurrentThreadPtr = next; 
}

