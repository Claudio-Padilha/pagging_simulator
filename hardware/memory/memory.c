#include "memory.h"

memory * newMemmory (int size)
{
    memory * ret = malloc(sizeof(memory));
    ret->size = size;
    ret->used = 0;
    ret->fr = newFrame(size);
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
}