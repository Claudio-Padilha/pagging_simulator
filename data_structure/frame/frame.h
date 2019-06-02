#ifndef FRAME
#define FRAME

#include <stdlib.h>
#include <pthread.h>
#include "../page/page.h"
#include "../process/process.h"

// Node structure used to control page queue for second chance algorithm.
typedef struct nodeFr
{
    page * p;
    struct nodeFr * next;
}nodeFr;

// Structure that has the map of frame ocuppation in memory.
typedef struct freeFrame
{
    int ffSize;                                     // Size of the structure.
    int * ff;                                      // ff is a vector of free frames. If ff[i] equals 1 frame i is free, else if ff[i] equals 0 it's not.

    // Queue used to control pages in memory.
    nodeFr * first;
    nodeFr * last; 
    int qSize;                                               // Queue current size.                                                                
}freeFrame; 
    
// Creates a new freeFrame structure. Receives the amount of available frames.
freeFrame * newFrame (int size);

// Creates newNodeFr
nodeFr * newNodeFr (process * p);
#endif