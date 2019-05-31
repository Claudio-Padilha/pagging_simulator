#include "timer.h"

timer * newTimer (int tq, int totalProc)
{
    timer * ret = malloc(sizeof(timer));
    ret->tq = tq;
    ret->totalProc = totalProc;
    ret->doneProc = 0;
    ret->tqCond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    ret->burstCond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return ret;
}