#include <stdio.h>
#include "texture.h"

tex2_t text2_clone(tex2_t* t) 
{
    tex2_t result = { t->u, t->v };
    return result;
}