#include "scheduler.h"

schArgs * newSchArgs (timer * t, queue * ready)
{
    schArgs * ret = malloc(sizeof(schArgs));
    ret->t = t;
    ret->ready = ready;

    return ret;
}

process * removeFromQueue (queue * ready)
{
    process * ret = ready->first->p;
    
    if (ready->first == ready->last)                                   // Only one process on queue
    {
        ready->last == NULL;
    }
    node * aux = ready->first;
    ready->first = ready->first->next;
    free(aux);

    return ret;
}

void * roundRobin (void * param)
{
    schArgs * sch = (schArgs *) param;                                                      

    while (1)                                                           
    {
        while (1)                                                           // Waits for a process to enter the ready queue if it is empty.
        {
            pthread_mutex_lock(&sch->ready->lock);
            if (sch->ready->first == NULL)                                  // Queue is empty.
            {
                pthread_mutex_unlock(&sch->ready->lock);
                continue;
            }else                                                           // Queue is not empty.
            {
                process * p = removeFromQueue (sch->ready);
                pthread_mutex_unlock(&sch->ready->lock);

                // TODO: shipper thread

                break;
            }
        }

        pthread_mutex_lock(&sch->t->lock);
        pthread_cond_wait (&sch->t->tqCond, &sch->t->lock);                 // Waits for timer to signal tq condition.
        pthread_mutex_unlock(&sch->t->lock);
    }
}