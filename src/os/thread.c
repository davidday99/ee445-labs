// thread.c 
// Thread implementation. 
// David Day
// Jan 12, 2023 
#include "thread.h"

#define THREAD_COUNT 10

static Tcb_Stack_t __attribute__ ((aligned (8))) Threads[THREAD_COUNT];
static uint16_t InUseBitField;

static int AllocateThread(void);

Tcb_t *Thread_Create() {
    int idx = AllocateThread();
    if (idx < 0) {
        return 0;
    } else {
        return &Threads[idx].tcb;
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
