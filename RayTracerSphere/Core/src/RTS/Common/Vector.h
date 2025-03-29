#pragma once

#include "glm/glm.hpp"

namespace RTS
{
	typedef glm::vec3 Vec3;
	typedef glm::vec4 Vec4;

	typedef Vec3 Color;
	typedef Vec4 Color4;
	typedef Vec3 Point;
	typedef Vec3 Vector;

	class VectorUtils
	{
	public:
		static bool NearZero(const Vec3& vec);
		static uint32_t ConvertToPixel(const Color& color);
		static uint32_t ConvertToPixel(const Color4& color);
		static Vector RandomInUnitDisk();
		static float GammaCorrection(float value, float gamma = 2.2f);
		static float ReinHardTonemap(float value);

	private:
		constexpr static float s_epsilon = 1e-8f;
	};
}