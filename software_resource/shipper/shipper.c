#include "shipper.h"

shArgs * newShArgs (process * p, disc *d, memory * m, timerS * t, int seed, int tq)
{
    shArgs * ret = malloc(sizeof(shArgs));
    ret->p = p;
    ret->seed = seed;
    ret->tq = tq;
    ret->d = d;
    ret->m = m;
    ret->t = t;
    
    return ret;
}

void * shipper (void * param)
{
    shArgs * sh = (shArgs *) param;

    srand(sh->seed);                                            
    int pgIndex = rand() % sh->p->numPgs;                                           // Gets the page index for this process.

    if (sh->p->pgTb[pgIndex]->valid == true)
    {
        sh->p->pgTb[pgIndex]->ref == true;                                          // Adjusts reference bit.

        pthread_mutex_lock(&sh->p->lock);

        int usage = cpu(sh->p, sh->tq);                                                         // consumes burst from process

        pthread_mutex_unlock(&sh->p->lock);

        // TODO: create timer thread
    }
}