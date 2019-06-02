#include "creator.h"

crArgs * newCrArgs (process * p, queue * ready)
{
    crArgs * ret = malloc(sizeof(crArgs));
    ret->p = p;
    ret->ready = ready;

    return ret;
}

int insertIntoQueueCr (process * p, queue * ready)
{
    node * n = newNode (p);
    if (ready->first == NULL)                                           // Queue is empty
    {
        ready->first = n;
    }else
    {
        ready->last->next = n;
    }
    

    ready->last = n;

    return p->id;
}

void * creator (void * param)
{
    crArgs * cr = (crArgs *) param;

    sleep(cr->p->creation);                                             // Wait for the moment to create process

    pthread_mutex_lock(&cr->ready->lock);                               // Aquires ready queue and process locks
    pthread_mutex_lock(&cr->p->lock);
    insertIntoQueueCr (cr->p, cr->ready);                                 // Insert process into ready queue.

    pthread_mutex_unlock(&cr->p->lock);                                   
    pthread_mutex_unlock(&cr->ready->lock);                             // Releases ready queue and process locks

    struct tm * currentTime;
    time_t segundos;
    time(&segundos);   
    currentTime = localtime(&segundos);
    printf("Time: %d:%d:%d - Criador de processos criou o processo %d e o colocou na fila de prontos.\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, cr->p->id);

    return NULL;
}