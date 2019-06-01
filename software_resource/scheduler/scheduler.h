#ifndef SCHEDULER
#define SCEHDULER

#include <pthread.h>
#include "../../data_structure/queue/queue.h"
#include "../timer/timer.h"
#include "../shipper/shipper.h"

// Structure used to pass arguments to scheduler.
typedef struct schArgs
{
    timer * t;
    queue * ready;                                                          // Ready queue that contains processes.
}schArgs;

// Creates new scArgs.
schArgs * newSchArgs (timer * t, queue * ready);

// Round Robin scheduler
void * roundRobin (void * param);
#endif