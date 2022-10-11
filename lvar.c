#include "lvar.h"
#include <string.h>

LVar *find_lvar(char *str, int len)
{
    for (LVar *lv = locals; lv; lv = lv->nextLVar)
    {
        if (lv->len == len && !memcmp(lv->name, str, lv->len))
        {
            return lv;
        }
    }
    return NULL;
}