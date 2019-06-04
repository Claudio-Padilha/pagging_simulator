#include "memory.h"

memory * newMemmory (int size)
{
    memory * ret = malloc(sizeof(memory));
    ret->size = size;
    ret->used = 0;
    ret->pgFaults = 0;
    ret->fr = newFrame(size);
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return ret;
}

void printFreeFrame (memory * m)
{
    printf("    FREE FRAME: ");
    for (int i = 0; i < m->fr->ffSize; i++)
    {
        if (i < m->fr->ffSize - 1)
        {
            printf("%d ", m->fr->ff[i]);
        }else
        {
             printf("%d\n", m->fr->ff[i]);
        }
    }
}