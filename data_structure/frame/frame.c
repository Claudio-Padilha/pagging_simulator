#include "frame.h"

freeFrame * newFrame (int size) 
{
    freeFrame * ret = (freeFrame *) malloc (sizeof(freeFrame));
    ret->ff = (int *) malloc (sizeof(int) * size);
    return ret;
}