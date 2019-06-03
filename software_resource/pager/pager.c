#include "pager.h"

pgArgs * newPgArgs (process * p, disc * d, memory * m, int pgIndx)
{
    pgArgs * ret = malloc(sizeof(pgArgs));
    ret->p = p;
    ret->d = d;
    ret->m = m;
    ret->pgIndx = pgIndx;
    return ret;
}

int hasFreeFrame (memory * m)
{
    for (int i = 0; i < m->size; i++)
    {
        if (m->fr->ff[i] == 1)                                      // Frame is free
        {
            return i;
        }
    }

    return -1;
}

int insertPage (page * p, memory * m)
{

    p->valid = true;                                                            // Page table adjustment (new page).
    p->ref = true;

    if (m->fr->qSize < m->fr->ffSize)                                           // Queue is not full.
    {

        nodeFr * n = malloc(sizeof(nodeFr));
        n->next = NULL;
        n->p = p;

        if (m->fr->first == NULL)                                           // Queue is empty.
        {
            m->fr->first = n;
        }
        else
        {
            m->fr->last->next = n;
        }
        m->fr->last = n;
        m->fr->qSize ++;

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
        
        m->fr->first->p->ref = false;
        m->fr->first->p->valid = false;                                   // Victm's page table adjustments.
        p->idf = m->fr->first->p->idf;                                    // New page table idf update.
        m->fr->first->p->idf = -1;

        m->fr->first->p = p;                                             // Changes pagens on the first node.

        m->fr->last->next = m->fr->first;                                    // put the first node on last position.
        m->fr->last = m->fr->first;

        m->fr->first = m->fr->first->next;                                   // Adjusts the new first node.
        m->fr->last->next = NULL;                                        // Adjusts new last next pointer.
    }

    return 0; 
}

void * pager (void * param)
{
    pgArgs * pgr = (pgArgs *) param;

    pthread_mutex_lock(&pgr->m->lock);
    pthread_mutex_lock(&pgr->d->lock);                              // Simbolic of getting page from disc
    pthread_mutex_lock(&pgr->p->lock);
    int fr = hasFreeFrame(pgr->m);

    if (fr >= 0)                                                   // Has a free frame
    {                                    
        // Allocated frame and updated pgTable (idf).

        pgr->m->fr->ff[fr] = 0;                                    
        pgr->p->pgTb[pgr->pgIndx]->idf = fr;
    }

    insertPage(pgr->p->pgTb[pgr->pgIndx], pgr->m);

    pthread_mutex_unlock(&pgr->m->lock);
    pthread_mutex_unlock(&pgr->d->lock);
    pthread_mutex_unlock(&pgr->p->lock);

    return NULL;
}