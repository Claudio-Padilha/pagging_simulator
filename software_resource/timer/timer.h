#ifndef TIMER
#define TIMER

#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include "../../data_structure/process/process.h"
#include "../../data_structure/queue/queue.h"
#include "../../hardware/memory/memory.h"

typedef struct timerS
{
    int tq;                                                 // Time quantum parameter.
    int totalProc;                                          // Total number of processes.
    int doneProc;                                           // Current number of processes that ended burst time.
                    
    pthread_mutex_t lock;
    pthread_cond_t tqCond;                                  // Condition variable to signal a process is done using CPU for this tq. Signals to Round Robin.
    pthread_cond_t endCond;                                 // Condition to terminate execution
}timerS;

typedef struct timerArgs
{
    timerS * t;
    process * p;
    queue * ready;
    memory * m;

    int usage;                                               // Amount of cpu used (time).
}timerArgs;

// Creates a new timer
timerS * newTimer (int tq, int totalProc);

// Creates new timer args.
timerArgs * newTimerArgs (timerS * t, process * p, int usage, queue * ready, memory * m);

int insertIntoQueueT (process * p, queue * ready);

// New timer thread.
void * timer (void * param);
#endif