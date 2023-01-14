#include "semaphore.h"

/* These implementations are in src/device as 
 * the are written in assembly. */
int sema_up(void *semaphore);
int sema_down(void *semaphore);

void SemaUp(semaphore_t *sema) {
    sema_up(&sema->count);
}

void SemaDown(semaphore_t *sema) {
    while (sema_down(&sema->count) == 0)
        ;
}

