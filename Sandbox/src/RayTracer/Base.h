#pragma once

#include <cmath>
#include <limits>
#include <memory>

constexpr float INF = std::numeric_limits<float>::infinity();
constexpr float PI = 3.1415926535897932385f;

// Utility functions
inline float DegreesToRadians(float degrees)
{
	return degrees * PI / 180.0f;
}

// Common headers
#include "RayTracer/Ray.h"
#include "RayTracer/Interval.h"