#pragma once

#include <cstdint>

inline uint32_t ConvertToPixel(float r, float g, float b)
{
	uint32_t red = static_cast<uint32_t>(r * 255.999f);
	uint32_t green = static_cast<uint32_t>(g * 255.999f);
	uint32_t blue = static_cast<uint32_t>(b * 255.999f);
	uint32_t pixel = 0xFF000000 | (red << 16) | (green << 8) | blue;
	return pixel;
}