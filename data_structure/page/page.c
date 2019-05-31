#include "page.h"

page * newPage()
{
    page * ret = malloc(sizeof(page));
    ret->idf = -1;
    ret->valid = false;
    ret->ref = false;
    return ret;
}