#ifndef EMATH_H
#define EMATH_H

#include <stdbool.h>

int igcd(int x, int y);
float lerp(float a, float b, float f);
bool CheckCollision2D(float positionX, float positionY, float scaleX, float scaleY, float targetPositionX, float targetPositionY, float targetScaleX, float targetScaleY);

#endif