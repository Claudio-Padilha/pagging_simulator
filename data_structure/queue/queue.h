#ifndef QUEUE
#define QUEUE

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "../process/process.h"

// Node struct for queue.
typedef struct node
{
    process * p;
    
    struct node * next;                                             // Next on queue.
}node;

// Queue struct
typedef struct queue
{
    node * first;                                                   // First element.
    node * last;                                                    // last element.

    pthread_mutex_t lock;                                           // Queue lock.
}queue;

// Creates new queue structure
queue * newQueue();

// Creates new node
node * newNode (process * p);

void printPgTable (queue * ready);
#endif