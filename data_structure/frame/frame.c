#include "frame.h"

freeFrame * newFrame (int size) 
{
    freeFrame * ret = malloc (sizeof(freeFrame));
    ret->ff = (int *) malloc (sizeof(int) * size);
    ret->first = NULL;
    ret->last = NULL;
    ret->ffSize = size;
    ret->qSize = 0;

    for (int i = 0; i < size; i ++)
    {
        ret->ff[i] = 1;                                                                         // All frames are free initially.
    }
    return ret;
}