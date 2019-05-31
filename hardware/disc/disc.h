#ifndef DISC
#define DISC

#include <pthread.h>
#include <stdlib.h>
#include "../../data_structure/page/page.h"

typedef struct node
{
    page * p;

    struct node * next;
}node;

// Structure representing disc
typedef struct disc
{
    node * storage;                                              // List of pages on disc. Points to the first element and each element points to next.                                                                
    pthread_mutex_t lock;                                        // Disc lock.
}disc;

// Creates new disc
disc * newDisc ();
#endif