#ifndef PAGER
#define PAGER

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../data_structure/queue/queue.h"
#include "../../hardware/disc/disc.h"
#include "../../hardware/memory/memory.h"

typedef struct pgArgs
{
    int pgIndx;                                              // Page index
    process * p;
    disc * d;
    memory * m;
}pgArgs;

// Creates new pager args
pgArgs * newPgArgs (process * p, disc * d, memory * m, int pgIndx);

// Creates new pager
void * pager (void * param);

// Verifies if there is a free frame on structure. if it has return the frame index, else returns -1.
int hasFreeFrame (memory * m);

// Inserts a page using second chance algorithm.
int insertPage (page * p, memory * m, int id, int pid);
#endif