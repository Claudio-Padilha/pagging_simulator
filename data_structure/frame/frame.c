#include "frame.h"

freeFrame * newFrame (int len) 
{
    freeFrame * ret = (freeFrame *) malloc (sizeof(freeFrame));
    ret->ff = (int *) malloc (sizeof(int) * len);
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    return ret;
}