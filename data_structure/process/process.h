#ifndef PROCESS
#define PROCESS

#include <stdlib.h>
#include <pthread.h>
#include "../page/page.h"
#include "../../hardware/disc/disc.h"

typedef struct process
{
    int id;   
    int numPgs;                                     // Number of process pages.    
    int creation;                                   // Creation time.
    int burst;    

    page ** pgTb;                                    // Vector containig reference to all pages necessary for this process (page table).

    pthread_mutex_t lock;
}process;

process * newProcess(int id, int numPgs, int creation, int burst, disc * d);
#endif