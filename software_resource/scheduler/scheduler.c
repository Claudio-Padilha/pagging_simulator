#include "scheduler.h"

schArgs * newSchArgs (timerS * t, queue * ready, memory * m, disc * d, int seed)
{
    schArgs * ret = malloc(sizeof(schArgs));
    ret->t = t;
    ret->ready = ready;
    ret->m = m;
    ret->d = d;
    ret->seed = seed;

    return ret;
}

process * removeFromQueue (queue * ready)
{
    if (ready->first == NULL)
    {
        return NULL;
    }
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

int insertIntoQueue (process * p, queue * ready)
{
    node * n = malloc(sizeof(node));
    n->p = p;
    n->next = NULL;
    if (ready->first == NULL)
    {
        ready->first = p;
    }
    else 
    {
        ready->last->next = n;
    }

    ready->last = n;

    return p->id;
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
                process * p = removeFromQueue (sch->ready);                 // Tries to get process with burst.

                while (p != NULL && p->burst == 0)                          // while processes have burst equal zero, erase it and update structures.
                {
                    pthread_mutex_lock(&sch->m->lock);

                    for (int i = 0; i<p->numPgs; i++)                       // Go through process' page.
                    {
                        node * aux;
                        node * prev;
                        if (p->pgTb[i]->valid == 1)                         // Process page is in memory.
                        {
                            sch->m->fr->ff[p->pgTb[i]->idf] = 1;            // Marks frame idf as free.
                            sch->m->used --;

                            aux = sch->m->fr->first;
                            while (aux != NULL && aux->p->idf != p->pgTb[i]->idf)           //Looks for process on free frame structure queue and removes it.
                            {
                                prev = aux;
                                aux = aux->next;
                            }

                            prev->next = aux->next;

                            if (aux->next == NULL)                                                  // It was the last on queue
                            {
                                sch->m->fr->last = prev;
                            }

                            sch->m->fr->qSize --;
                        }
                    }

                    free(p);                                                // Deletes process and page table

                    pthread_mutex_unlock(&sch->m->lock);

                    p = removeFromQueue (sch->ready);                       // Looks for a process with burst time and remove if the porcess has burst equal 0
                }

                if (p == NULL)                                              // No process with burst time.
                {
                    pthread_mutex_unlock(&sch->ready->lock);
                    continue;
                }

                insertIntoQueue(p, sch->ready);                             // Put process back into ready queue.
                pthread_mutex_unlock(&sch->ready->lock);

                pthread_t sh;
                shArgs * shAr = newShArgs(p, sch->d, sch->m, sch->t, sch->seed, sch->t->tq);

                pthread_create (&sh, NULL, shipper, (void *) shAr);                                             // Sends process to shipper.

                break;
            }
        }

        pthread_mutex_lock(&sch->t->lock);
        pthread_cond_wait (&sch->t->tqCond, &sch->t->lock);                 // Waits for timer to signal tq condition.
        pthread_mutex_unlock(&sch->t->lock);
    }

    return NULL;
}