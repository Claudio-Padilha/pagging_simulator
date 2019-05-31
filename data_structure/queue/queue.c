#include "queue.h"

queue * newQueue()
{
    queue * ret = malloc(sizeof(queue));
    ret->first = NULL;
    ret->last = NULL;
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
}
