#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

typedef struct semaphore {
  long count;   
} semaphore_t;

int sema_up(void *semaphore);
int sema_down(void *semaphore);

#endif /* __SEMAPHORE_H__ */

