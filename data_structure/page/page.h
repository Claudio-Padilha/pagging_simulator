#ifndef PAGE
#define PAGE

#include <stdlib.h>
#include <stdbool.h>

typedef struct page
{
    int idf;                                        // Frame in memory that page is currently located. -1 if not in memory.
    bool valid;                                     // Valid / invalid bit.
    bool ref;                                       // Reference bit.
}page;

// Creates a new page;
page * newPage();
#endif