#include "disc.h"

disc * newDisc ()
{
    disc * ret = malloc(sizeof(disc));
    ret->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    return ret;
}