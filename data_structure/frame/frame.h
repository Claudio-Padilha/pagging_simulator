#ifndef FRAME
#define FRAME

#include <stdlib.h>
#include <pthread.h>

// Structure that has the map of frame ocuppation in memory
typedef struct freeFrame
{
    int * ff;                                          // ff is a vector of free frames. If ff[i] equals 1 frame is free, else if ff[i] equals 0 it's not.
    pthread_mutex_t lock;                              // Free frame structure lock.
}freeFrame; 
    
// Creates a new freeFrame structure. Receives the amount of available frames.
freeFrame * newFrame (int len);
#endif