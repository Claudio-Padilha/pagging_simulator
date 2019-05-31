#include <process.h>

process * newProcess(int pid, int numPgs, int creation, int burst)
{
    process * ret = malloc(sizeof(process));
    ret->id = pid;
    ret->numPgs = numPgs;
    ret->creation = creation;
    ret->burst = burst;
    ret->pgTb = malloc(sizeof(page)*numPgs);

    for (int i = 0; i < numPgs; i++)                                                // Creates page table
    {
        ret->pgTb[i] = *(newPage());
    }
}