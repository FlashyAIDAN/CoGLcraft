#include "emath.h"

int igcd(int x, int y)
{
    if(x == 0 || y == 0)
        return 0;

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

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

bool CheckCollision2D(float positionX, float positionY, float scaleX, float scaleY, float targetPositionX, float targetPositionY, float targetScaleX, float targetScaleY)
{
    if 
    (
        positionX - (scaleX / 2.0f) <= targetPositionX + (targetScaleX / 2.0f) &&
        positionX + (scaleX / 2.0f) >= targetPositionX - (targetScaleX / 2.0f) &&
        positionY - (scaleY / 2.0f) <= targetPositionY + (targetScaleY / 2.0f) &&
        positionY + (scaleY / 2.0f) >= targetPositionY - (targetScaleY / 2.0f)
    )
        return true;
    else
        return false;
}