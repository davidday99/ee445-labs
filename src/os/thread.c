// thread.c 
// Thread implementation. 
// David Day
// Jan 12, 2023 
#include "thread.h"

#define THREAD_COUNT 10

static Tcb_Stack_t __attribute__ ((aligned (8))) Threads[THREAD_COUNT];
static uint16_t InUseBitField;
static uint32_t Thread_Id;

static int AllocateThread(void);

Tcb_Stack_t *Thread_Create() {
    int idx = AllocateThread();
    if (idx < 0) {
        return 0;
    } else {
        Threads[idx].tcb.sp = (unsigned long) (&Threads[idx] + sizeof(Tcb_Stack_t));
        Threads[idx].tcb.id = ++Thread_Id;  // IDs start at 1
        Threads[idx].tcb.next = Threads[idx].tcb.prev = 0;
        Threads[idx].tcb.priority = 0;
        Threads[idx].tcb.state = IDLE;
        return &Threads[idx];
    } 
}

int Thread_Destroy(Tcb_t *t) {
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (t == &Threads[i].tcb) {
            InUseBitField &= (unsigned long) ~(1 << ((THREAD_COUNT - 1) - i));
            return 1;
        }
    }
    return 0;
}

static int AllocateThread(void) {
    for (int i = 0; i < THREAD_COUNT; i++) {
        if ((InUseBitField & (unsigned long) (1 << ((THREAD_COUNT - 1) - i))) == 0) {
            InUseBitField |= (unsigned long) (1 << ((THREAD_COUNT - 1)- i));
            return i;
        }
    }
    return -1;
}
