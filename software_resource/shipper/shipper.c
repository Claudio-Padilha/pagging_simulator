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

    if (sh->p->pgTb[pgIndex]->valid == true)                                        // There is no page fault.
    {
        sh->p->pgTb[pgIndex]->ref == true;                                          // Adjusts reference bit.

        pthread_mutex_lock(&sh->p->lock);

        int usage = cpu(sh->p, sh->tq);                                                         // consumes burst from process.

        pthread_mutex_unlock(&sh->p->lock);

        timerArgs * tmArgs = newTimerArgs (sh->t, sh->p, usage);
        pthread_t tm;

        pthread_create (&tm, NULL, timer, (void *) tmArgs);
    }
    else                                                                            // Page fault.
    {
        pgArgs * pgAr = newPgArgs (sh->p, sh->d, sh->m, pgIndex);
        pthread_t pgr;
        pthread_create(&pgr, NULL, pager, (void *) pgAr);                           // Calls pager to bring the missing page to memory.
        pthread_join(pgr, NULL);                                                    // wait for pager to bring missing page to memory.


        pthread_mutex_lock(&sh->p->lock);

        int usage = cpu(sh->p, sh->tq);                                                         // consumes burst from process.

        pthread_mutex_unlock(&sh->p->lock);

        timerArgs * tmArgs = newTimerArgs (sh->t, sh->p, usage);
        pthread_t tm;

        pthread_create (&tm, NULL, timer, (void *) tmArgs);
    }
    

    return NULL;
}