#include "process.h"

process * newProcess(int pid, int numPgs, int creation, int burst, disc * d)
{
    process * ret = malloc(sizeof(process));
    ret->id = pid;
    ret->numPgs = numPgs;
    ret->creation = creation;
    ret->burst = burst;
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    ret->pgTb = (page **) malloc(sizeof(page *)*numPgs);

    for (int i = 0; i<numPgs; i++)
    {
        ret->pgTb[i] = (page *) malloc(sizeof(page));
        ret->pgTb[i]->idf = -1;
        ret->pgTb[i]->ref = false;
        ret->pgTb[i]->ref = false;
        ret->pgTb[i]->id = i;
    }

    for (int i = 0; i < numPgs; i++)                                                // Creates page table.
    {
        pthread_mutex_lock(&d->lock);                                               // Aquires disc lock to simulate storage on disc.
        ret->pgTb[i] = newPage(i);
        pthread_mutex_unlock(&d->lock);                                                   
    }                                                                               

    return ret;
}