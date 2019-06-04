#include "queue.h"

queue * newQueue()
{
    queue * ret = malloc(sizeof(queue));
    ret->first = NULL;
    ret->last = NULL;
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return ret;
}

node * newNode (process * p)
{
    node * ret = malloc(sizeof(node));
    ret->next = NULL;
    ret->p = p;
    return ret;
}

void printPgTable (queue * ready)
{
    node * aux = ready->first;

    while (aux != NULL)
    {
        if (aux->p->burst > 0)
        {
            for (int i = 0; i < aux->p->numPgs; i++)
            {
                printf("    PROCESS: %d PGID: %d, FRAME: %d, REF: %d, VALID: %d\n", aux->p->id, aux->p->pgTb[i]->id, aux->p->pgTb[i]->idf, aux->p->pgTb[i]->ref, aux->p->pgTb[i]->valid);
            }
            printf("\n");
        }

        aux = aux->next;
    }
}