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
}

