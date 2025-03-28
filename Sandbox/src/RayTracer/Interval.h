#pragma once

#include "RayTracer/Base.h"

namespace RT
{
	class Interval
	{
	public:
		float Min, Max;

		Interval(): Min(INF), Max(-INF) {} // Default interval is empty
		Interval(float min, float max) : Min(min), Max(max) {}

		float size() const { return Max - Min; }
		bool contains(float x) const { return x >= Min && x <= Max; }
		bool surrounds(float x) const { return x > Min && x < Max; }
		float clamp(float x) const { return glm::clamp(x, Min, Max); }

		static const Interval Empty, Universe;
	};
}