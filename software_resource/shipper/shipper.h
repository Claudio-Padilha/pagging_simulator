#ifndef SHIPPER
#define SHIPPER

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../data_structure/process/process.h"
#include "../../hardware/cpu/cpu.h"
#include "../timer/timer.h"
#include "../../hardware/memory/memory.h"
#include "../pager/pager.h"

typedef struct shArgs
{
    int seed;                                           // Used to generate the sequence of a process accesses to its pages.
    int tq;

    process * p;
    timerS * t;
    disc * d;
    memory * m;
}shArgs;

// Creates new shipper args.
shArgs * newShArgs (process * p, disc * d, memory *m, timerS * t, int seed, int tq);

// Creates a new shipper.
void * shipper (void * param);
#endif