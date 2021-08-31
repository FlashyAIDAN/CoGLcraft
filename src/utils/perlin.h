#ifndef PERLIN_H
#define PERLIN_H

// From: https://gist.github.com/nowl/828013
// TODO(Aidan): Convert to float

#include <math.h>

static int Noise2(int seed, int x, int y);

static double LinInter(double x, double y, double s);
static double SmoothInter(double x, double y, double s);
static double Noise2D(int seed, double x, double y);
extern double PerlinGet2D(int seed, double x, double y, double freq, int depth);

#endif