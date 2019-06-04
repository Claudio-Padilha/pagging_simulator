#ifndef MEMORY
#define MEMORY

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../data_structure/frame/frame.h"

// Structure representing memory.
typedef struct memory
{
    int size;
    int used;
    int pgFaults;

    freeFrame * fr;                                                 // Free frame control structure.
    pthread_mutex_t lock;                                           // Memory lock.
}memory;

// Creates new memory.
memory * newMemmory (int size);

void printFreeFrame (memory * m);

#endif