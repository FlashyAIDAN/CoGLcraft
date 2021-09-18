#include "emath.h"

int igcd(int x, int y)
{
    x = ( x > 0) ? x : -x;
    y = ( y > 0) ? y : -y;

    while(x != y)
    {
        if(x > y)
            x -= y;
        else
            y -= x;
    }
    return x;
}