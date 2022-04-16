#pragma once
#include "commons.h"

#ifndef PERLIN_H
#define PERLIN_H

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */


float interpolate(float a0, float a1, float w);

sf::Vector2f randomGradient(int ix, int iy, float seed);
// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y, float seed);

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y, float seed);



#endif