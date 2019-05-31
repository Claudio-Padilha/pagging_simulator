#ifndef MEMORY
#define MEMORY

#include "../../data_structure/frame/frame.h"
#include <pthread.h>
#include <stdlib.h>

// Structure representing memory.
typedef struct memory
{
    int size;
    int used;

    freeFrame * fr;                                                 // Free frame control structure.
    pthread_mutex_t lock;                                           // Memory lock.
}memory;

// Creates new memory.
memory * newMemmory (int size);
#endif