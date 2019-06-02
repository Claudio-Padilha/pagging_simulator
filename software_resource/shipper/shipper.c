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
    struct tm * currentTime;
    time_t segundos;

    srand(sh->seed);                                            
    int pgIndex = rand() % sh->p->numPgs;                                           // Gets the page index for this process.

    if (sh->p->pgTb[pgIndex]->valid == true)                                        // There is no page fault.
    {

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Despachante percebe que a pagina %d do processo %d esta na memoria.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, pgIndex, sh->p->id);

        sh->p->pgTb[pgIndex]->ref = true;                                          // Adjusts reference bit.

        pthread_mutex_lock(&sh->p->lock);

        int usage = cpu(sh->p, sh->tq);                                                         // consumes burst from process.

        pthread_mutex_unlock(&sh->p->lock);

        timerArgs * tmArgs = newTimerArgs (sh->t, sh->p, usage);
        pthread_t tm;

        pthread_create (&tm, NULL, timer, (void *) tmArgs);

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Despachante reiniciou o Timer com %d e liberou a CPU ao processo %d.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, usage, sh->p->id);
        
    }
    else                                                                            // Page fault.
    {
        pthread_mutex_lock(&sh->m->lock);
        sh->m->pgFaults ++;                                                         // Update total pg faults
        pthread_mutex_lock(&sh->m->lock);

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Despachante percebe que a pagina %d do processo %d nao esta na memoria e solicita que o Pager traga %d a memoria.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, pgIndex, sh->p->id, pgIndex);

        pgArgs * pgAr = newPgArgs (sh->p, sh->d, sh->m, pgIndex);
        pthread_t pgr;
        pthread_create(&pgr, NULL, pager, (void *) pgAr);                           // Calls pager to bring the missing page to memory.
        pthread_join(pgr, NULL);                                                    // wait for pager to bring missing page to memory.

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Despachante percebe que a pagina %d do processo %d nao esta na memoria e solicita que o Pager traga %d a memoria.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, pgIndex, sh->p->id, pgIndex);

        pthread_mutex_lock(&sh->p->lock);

        int usage = cpu(sh->p, sh->tq);                                                         // consumes burst from process.

        pthread_mutex_unlock(&sh->p->lock);

        timerArgs * tmArgs = newTimerArgs (sh->t, sh->p, usage);
        pthread_t tm;

        pthread_create (&tm, NULL, timer, (void *) tmArgs);

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Despachante reiniciou o Timer com %d e liberou a CPU ao processo %d.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, usage, sh->p->id);
    }
    

    return NULL;
}