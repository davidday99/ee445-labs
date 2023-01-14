#ifndef __SCHED_H__
#define __SCHED_H__

#include "thread.h"

Tcb_t* Sched_GetCurrentThread(void);
int Sched_AddThread(Tcb_t *t);
int Sched_RemoveThread(Tcb_t *t);

#endif /* __SCHED_H__ */

