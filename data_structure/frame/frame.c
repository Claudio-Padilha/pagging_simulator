#include "frame.h"

freeFrame * newFrame (int size) 
{
    freeFrame * ret = (freeFrame *) malloc (sizeof(freeFrame));
    ret->ff = malloc (sizeof(int) * size);
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