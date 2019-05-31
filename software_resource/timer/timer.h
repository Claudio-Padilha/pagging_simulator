#ifndef TIMER
#define TIMER

#include <pthread.h>
#include <stdlib.h>

typedef struct timer
{
    int tq;                                                 // Time quantum parameter.
    int totalProc;                                          // Total number of processes.
    int doneProc;                                           // Current number of processes that ended burst time.

    pthread_mutex_t lock;
    pthread_cond_t tqCond;                                  // Condition variable to signal a process is done using CPU for this tq. Signals to Round Robin.
    pthread_cond_t burstCond;                               // Condition variable to signal that a process' burst time ended.
}timer;

// Creates a new timer
timer * newTimer (int tq, int totalProc);
#endif