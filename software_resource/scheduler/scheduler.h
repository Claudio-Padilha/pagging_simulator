#ifndef SCHEDULER
#define SCEHDULER

#include <pthread.h>
#include "../../data_structure/queue/queue.h"
#include "../timer/timer.h"
#include "../shipper/shipper.h"

// Structure used to pass arguments to scheduler.
typedef struct schArgs
{
    timerS * t;
    queue * ready;                                                          // Ready queue
    memory * m;
    disc * d;

    int seed;
}schArgs;

// Creates new scArgs.
schArgs * newSchArgs (timerS * t, queue * ready, memory * m, disc * d, int seed);

// Round Robin scheduler
void * roundRobin (void * param);

// Inserts a process back into ready queue
int insertIntoQueue (process * p, queue * ready);
#endif