#ifndef CREATOR
#define CREATOR

#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../../data_structure/process/process.h"
#include "../../data_structure/queue/queue.h"

// Struct to pass arguments to creator.
typedef struct crArgs
{   
    // Process.    
    process * p;

    // Ready queue.
    queue * ready;
}crArgs;

// Creates a new set of arguments for creator.
crArgs * newCrArgs (process * p, queue * ready);

// Creates the process at a given time and puts it into the ready queue.
void * creator (void * param);

//  inserts a process into ready queue and returns its pid.
int insertIntoQueue (process * p, queue * ready);
#endif