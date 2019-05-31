#include "creator.h"

crArgs * newCrArgs (process * p, queue * ready)
{
    crArgs * ret = malloc(sizeof(crArgs));
    ret->p = p;
    ret->ready = ready;
}

int insertIntoQueue (process * p, queue * ready)
{
    node * n = newNode (p);
    if (ready->first == NULL)                                           // Queue is empty
    {
        ready->first = n;
    }

    ready->last->next = n;
    ready->last = n;

    return p->id;
}

void * creator (void * param)
{
    crArgs * cr = (crArgs *) param;

    sleep(cr->p->creation);                                             // Wait for thr moment to create process

    pthread_mutex_lock(&cr->ready->lock);                               // Aquires ready queue lock
    
    insertIntoQueue (cr->p, cr->ready);                                 // Insert process into ready queue.

    pthread_mutex_unlock(&cr->ready->lock);                             // Releases ready queue lock
}