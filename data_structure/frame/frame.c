#include "frame.h"

freeFrame * newFrame (int size) 
{
    freeFrame * ret = (freeFrame *) malloc (sizeof(freeFrame));
    ret->ff = (int *) malloc (sizeof(int) * size);
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    return ret;
}