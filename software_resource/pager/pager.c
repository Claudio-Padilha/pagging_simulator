#include "pager.h"

pgArgs * newPgArgs (process * p, disc * d, memory * m, int pgIndx, queue * ready)
{
    pgArgs * ret = malloc(sizeof(pgArgs));
    ret->p = p;
    ret->d = d;
    ret->m = m;
    ret->pgIndx = pgIndx;
    ret->ready = ready;
    return ret;
}

int hasFreeFrame (memory * m)
{
    for (int i = 0; i < m->fr->ffSize; i++)
    {
        if (m->fr->ff[i] == 1)                                      // Frame is free
        {
            return i;
        }
    }

    return -1;
}

int insertPage (page * p, memory * m, int id, int pid, queue * ready)
{

    struct tm * currentTime;
    time_t segundos;

    p->valid = true;                                                            // Page table adjustment (new page).
    p->ref = true;

    if (m->fr->qSize < m->fr->ffSize)                                           // Queue is not full.
    {

        nodeFr * n = malloc(sizeof(nodeFr));
        n->next = NULL;
        n->p = p;

        int fr = hasFreeFrame(m);

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Pager percebe que ha quadro livre %d.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, fr);

        if (m->fr->first == NULL)                                           // Queue is empty.
        {
            m->fr->first = n;
        }
        else
        {
            m->fr->last->next = n;
        }
        p->idf = fr;
        m->fr->ff[fr] = 0;
        m->fr->last = n;
        m->fr->qSize ++;

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Pager le do disco a pagina %d solicitada e o coloca no quadro livre %d.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, id, fr);
        printFreeFrame(m);
        printPgTable(ready);

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Despachante eh avisado pelo Pager que a pagina %d do processo %d esta no quadro %d.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, id, pid, fr);

    }
    else                                                                // Queue is full.
    {
        while (m->fr->first->p->ref == true)                             // While the first page(s) ref bit is valid, look for one page that its ref is not.
        {
            m->fr->first->p->ref = false;                                     // Adjusts reference bit.
            
            m->fr->last->next = m->fr->first;                                    // put the first node on last position.
            m->fr->last = m->fr->first;

            m->fr->first = m->fr->first->next;                                   // Adjusts the new first node.
            m->fr->last->next = NULL;                                        // Adjusts new last next pointer.
        }

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Pager percebe que nao ha quadro livre e substitui a pagina %d que esta no quadro %d.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec,m->fr->first->p->id, m->fr->first->p->idf);
        printFreeFrame(m);
        printPgTable(ready);
        
        m->fr->first->p->valid = false;                                   // Victm's page table adjustments.
        p->idf = m->fr->first->p->idf;                                    // New page table idf update.
        m->fr->first->p->idf = -1;

        m->fr->first->p = p;                                             // Changes page on the first node.

        m->fr->last->next = m->fr->first;                                    // put the first node on last position.
        m->fr->last = m->fr->first;

        m->fr->first = m->fr->first->next;                                   // Adjusts the new first node.
        m->fr->last->next = NULL;                                        // Adjusts new last next pointer.

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Pager avisa o Despachante que o processo %d esta na memoria.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, pid);

        time(&segundos);   
        currentTime = localtime(&segundos);
        printf("Time: %d:%d:%d - Despachante eh avisado pelo Pager que a pagina %d do processo %d esta no quadro %d.\n"
        , currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, id, pid, p->idf);
    }

    return 0; 
}

void * pager (void * param)
{
    pgArgs * pgr = (pgArgs *) param;

    pthread_mutex_lock(&pgr->m->lock);
    pthread_mutex_lock(&pgr->d->lock);                              // Simbolic of getting page from disc
    pthread_mutex_lock(&pgr->p->lock);
    pthread_mutex_lock(&pgr->ready->lock);
    int fr = hasFreeFrame(pgr->m);

    if (fr >= 0)                                                   // Has a free frame
    {                                    
        // Allocated frame and updated pgTable (idf).
                                 
        pgr->p->pgTb[pgr->pgIndx]->idf = fr;
    }

    insertPage(pgr->p->pgTb[pgr->pgIndx], pgr->m, pgr->pgIndx, pgr->p->id, pgr->ready);

    pthread_mutex_unlock(&pgr->m->lock);
    pthread_mutex_unlock(&pgr->d->lock);
    pthread_mutex_unlock(&pgr->p->lock);
    pthread_mutex_unlock(&pgr->ready->lock);

    return NULL;
}