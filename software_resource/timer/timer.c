#include "timer.h"

timerS * newTimer (int tq, int totalProc)
{
    timerS * ret = malloc(sizeof(timerS));
    ret->tq = tq;
    ret->totalProc = totalProc;
    ret->doneProc = 0;
    ret->tqCond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    ret->endCond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return ret;
}

timerArgs * newTimerArgs (timerS * t, process * p, int usage, queue * ready, memory * m)
{
    timerArgs * ret = malloc(sizeof(timerArgs));
    ret->t = t;
    ret->p = p;
    ret->usage = usage;
    ret->ready = ready;
    ret->m = m;

    return ret;
}

int insertIntoQueueT (process * p, queue * ready)
{
    node * n = malloc(sizeof(node));
    n->p = p;
    n->next = NULL;
    if (ready->first == NULL)
    {
        ready->first = n;
    }
    else 
    {
        ready->last->next = n;
    }

    ready->last = n;
    return p->id;
}

void * timer (void * param)
{
    timerArgs * tm = (timerArgs *) param;

    sleep(tm->usage);

    struct tm * currentTime;
    time_t segundos;

    pthread_mutex_lock(&tm->ready->lock);
    insertIntoQueueT(tm->p, tm->ready);
    pthread_mutex_unlock(&tm->ready->lock);

    if (tm->p->burst == 0)
    {
        tm->t->doneProc ++;

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - O processo %d terminou sua execução.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, tm->p->id);

        pthread_mutex_lock(&tm->ready->lock);
        pthread_mutex_lock(&tm->m->lock);

        printFreeFrame(tm->m);
        printPgTable(tm->ready);

        pthread_mutex_unlock(&tm->ready->lock);
        pthread_mutex_unlock(&tm->m->lock);
    }

    pthread_cond_broadcast(&tm->t->tqCond);                                                 // Signals to Round Robin

    time(&segundos);   
    currentTime = localtime(&segundos);
    printf("Time: %d:%d:%d - Timer informa  ao Escalonador  Round-Robin  de  CPU que  o  processo %d atualmente  em  execucao precisa ser retirado da CPU.\n"
    , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, tm->p->id);
 
    if (tm->t->doneProc == tm->t->totalProc)                                                // All processes ended
    {
        pthread_cond_broadcast(&tm->t->endCond);
    }

    return NULL;
}