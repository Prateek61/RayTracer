#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include "Engine.h"

namespace RT::Utils
{
	class Image
	{
	public:
		static uint32_t ConvertToPixel(float r, float g, float b, float a = 1.0);
		static uint32_t ConvertToPixel(const glm::vec4& color);
		static void SaveImage(uint32_t width, uint32_t height, Engine::Buffer imageBuffer, const std::filesystem::path& filePath);
		static float LinearToGamma(float linear, float gamma = 0.454545f);
		static glm::vec4 LinearToGamma(const glm::vec4& linear, float gamma = 0.454545f);
	};

	static glm::vec3 RandomOnHemisphere(const glm::vec3& normal)
	{
		auto on_unit_sphere = Engine::Random::InUnitSphere();
		if (glm::dot(on_unit_sphere, normal) > 0.0f)
		{
			return on_unit_sphere;
		}
		else
		{
			return -on_unit_sphere;
		}
	}

	static bool NearZero(const glm::vec3& vec)
	{
		constexpr float s = 1e-6f;
		return (std::fabs(vec.r) < s) && (std::fabs(vec.g) < s) && (std::fabs(vec.b) < s);
	}
}

