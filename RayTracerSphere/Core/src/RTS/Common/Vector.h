#pragma once

#include "glm/glm.hpp"
#include "Engine.h"

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
		inline static bool NearZero(const Vec3& vec);
		inline static uint32_t ConvertToPixel(const Color& color);
		inline static uint32_t ConvertToPixel(const Color4& color);
		inline static Vector RandomInUnitDisk();
		inline static float GammaCorrection(float value, float gamma = 2.2f);
		inline static float ReinHardTonemap(float value);

	private:
		constexpr static float s_epsilon = 1e-8f;
	};
}

// Inline definitions
namespace RTS
{
	bool VectorUtils::NearZero(const Vec3& vec)
	{
		return (std::fabs(vec.r) < s_epsilon) && (std::fabs(vec.g) < s_epsilon) && (std::fabs(vec.b) < s_epsilon);
	}

	uint32_t VectorUtils::ConvertToPixel(const Color& color)
	{
		return ConvertToPixel(Color4(color, 1.0f));
	}

	uint32_t VectorUtils::ConvertToPixel(const Color4& color)
	{
		const uint32_t red = static_cast<uint32_t>(glm::clamp(color.r, 0.0f, 0.999f) * 256.0f);
		const uint32_t green = static_cast<uint32_t>(glm::clamp(color.g, 0.0f, 0.999f) * 256.0f);
		const uint32_t blue = static_cast<uint32_t>(glm::clamp(color.b, 0.0f, 0.999f) * 256.0f);
		const uint32_t alpha = static_cast<uint32_t>(glm::clamp(color.a, 0.0f, 0.999f) * 256.0f);
		const uint32_t pixel = (alpha << 24) | (blue << 16) | (green << 8) | red;
		return pixel;
	}

	Vector VectorUtils::RandomInUnitDisk()
	{
		while (true)
		{
			auto p = Vector{ Engine::Random::Float(-1.0f, 1.0f), Engine::Random::Float(-1.0f, 1.0f), 0.0f };
			if (glm::dot(p, p) >= 1.0f)
			{
				return p;
			}
		}
	}

	float VectorUtils::GammaCorrection(float value, float gamma)
	{
		return std::pow(value, 1.0f / gamma);
	}

	float VectorUtils::ReinHardTonemap(float value)
	{
		return value / (value + 1.0f);
	}
}