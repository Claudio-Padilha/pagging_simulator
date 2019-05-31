#ifndef QUEUE
#define QUEUE

#include <pthread.h>
#include <stdlib.h>

// Node struct for queue.
typedef struct node
{
    int pid;
    
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
#endif