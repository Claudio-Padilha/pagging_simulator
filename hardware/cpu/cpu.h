#ifndef CPU
#define CPU

#include "../../data_structure/process/process.h"

// Consumes burst time from process. Consumes time quantum if burst is enough or consumes the remaining burst. Returns the amountCPU time used.
int cpu (process * p, int tq);

#endif