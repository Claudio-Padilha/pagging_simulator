#include "timer.h"

timerS * newTimer (int tq, int totalProc)
{
    timerS * ret = malloc(sizeof(timer));
    ret->tq = tq;
    ret->totalProc = totalProc;
    ret->doneProc = 0;
    ret->tqCond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    ret->endCond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return ret;
}

timerArgs * newTimerArgs (timerS * t, process * p, int usage)
{
    timerArgs * ret = malloc(sizeof(timerArgs));
    ret->t = t;
    ret->p = p;
    ret->usage = usage;

    return ret;
}

void * timer (void * param)
{
    timerArgs * tm = (timerArgs *) param;

    sleep(tm->usage);

    if (tm->p->burst == 0)
    {
        tm->t->doneProc ++;
    }

    pthread_cond_broadcast(&tm->t->tqCond);                                                 // Signals to Round Robin

    if (tm->t->doneProc == tm->t->totalProc)                                                // All processes ended
    {
        pthread_cond_broadcast(&tm->t->endCond);
    }
}