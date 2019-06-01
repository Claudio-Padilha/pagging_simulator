#ifndef DISC
#define DISC

#include <pthread.h>
#include <stdlib.h>

// Structure representing disc. The processes and pages doesn't need to go to the disk, beacuse valid bit on each page, 
// will simulate the necessary disk features. It has a disk lock for integrity prpouse.
typedef struct disc
{   
    pthread_mutex_t lock;                                        // Disc lock.
}disc;

// Creates new disc
disc * newDisc ();
#endif