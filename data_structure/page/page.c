#include "page.h"

page * newPage(int id)
{
    page * ret = malloc(sizeof(page));
    ret->idf = -1;
    ret->valid = false;
    ret->ref = false;
    ret->id = id;
    return ret;
}