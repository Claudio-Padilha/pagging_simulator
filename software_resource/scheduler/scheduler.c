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
        ready->last = NULL;
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
        ready->first = n;
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

                    for (int i = 0; i < p->numPgs; i++)                       // Go through process' page.
                    {
                        nodeFr * aux = (nodeFr *) NULL;
                        nodeFr * prev  = (nodeFr *) NULL;
                        if (p->pgTb[i]->valid == true)                         // Process page is in memory.
                        {
                            sch->m->fr->ff[p->pgTb[i]->idf] = 1;            // Marks frame idf as free.
                            p->pgTb[i]->valid = false;
                            p->pgTb[i]->ref = false;

                            aux = sch->m->fr->first;
                            prev = NULL;
                            while (aux != NULL && aux->p->idf != p->pgTb[i]->idf)           //Looks for page on free frame structure queue and removes it.
                            {
                                prev = aux;
                                aux = aux->next;
                            }

                            if (prev != NULL)
                            {
                                prev->next = aux->next;
                            }else
                            {
                                sch->m->fr->first = NULL;                                           //only element on queue
                                sch->m->fr->last = NULL;
                            }
                            
                            aux->p->idf = -1;

                            if (aux->next == NULL)                                                  // It was the last on queue
                            {
                                sch->m->fr->last = prev;
                            }

                            sch->m->fr->qSize --;
                        }
                    }

                    process * aux = p;
                    free(aux);                                                // Deletes process and page table

                    pthread_mutex_unlock(&sch->m->lock);

                    p = removeFromQueue (sch->ready);                       // Looks for a process with burst time and remove if the porcess has burst equal 0
                }

                if (p == NULL)                                              // No process with burst time.
                {
                    pthread_mutex_unlock(&sch->ready->lock);
                    continue;
                }

                insertIntoQueue(p, sch->ready);

                pthread_mutex_unlock(&sch->ready->lock);

                pthread_t sh;
                shArgs * shAr = newShArgs(p, sch->d, sch->m, sch->t, sch->seed, sch->t->tq);

                struct tm * currentTime;
                time_t segundos;
                time(&segundos);   
                currentTime = localtime(&segundos);
                printf("Time: %d:%d:%d - Escalonador Round-Robin de CPU escolheu o processo %d, retirou-o da fila de prontos e o encaminhou ao Despachante.\n"
                , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, p->id);

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