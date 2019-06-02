#include "process.h"

process * newProcess(int pid, int numPgs, int creation, int burst, disc * d)
{
    process * ret = malloc(sizeof(process));
    ret->id = pid;
    ret->numPgs = numPgs;
    ret->creation = creation;
    ret->burst = burst;
    ret->pgTb = malloc(sizeof(page *)*numPgs);
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < numPgs; i++)                                                // Creates page table.
    {
        pthread_mutex_lock(&d->lock);                                               // Aquires disc lock to simulate storage on disc.
        ret->pgTb[i] = newPage();
        pthread_mutex_unlock(&d->lock);                                                   
    }                                                                               

    return ret;
}