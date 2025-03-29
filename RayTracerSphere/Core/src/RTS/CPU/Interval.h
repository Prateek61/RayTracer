#pragma once

#include "RTS/Common/Base.h"

namespace RTS
{
	class Interval
	{
	public:
		float Min, Max;

		Interval() : Min(RTS::INF), Max(-RTS::INF) {}
		Interval(float min, float max) : Min(min), Max(max) {}

		float Length() const { return Max - Min; }
		bool Contains(float value) const { return value >= Min && value <= Max; }
		bool Contains(const Vector& vec) const { return Contains(vec.x) && Contains(vec.y) && Contains(vec.z); }
		bool Surrounds(float value) const { return value > Min && value < Max; }

		template<typename T>
		T Clamp(const T& value) const
		{
			return glm::clamp(value, Min, Max);
		}

		static const Interval Empty, Universe;
	};
}