#pragma once

#include "RTS/Common/Base.h"

#include <string>
#include <cstdint>
#include <vector>

namespace RTS
{
	class Scene;

	struct Sphere
	{
		Point Center{ 0.0f };
		float Radius{ 1.0f };
		uint32_t MaterialID{ 0 };
		std::string Name = "SampleSphere";

		bool ImGuiProperties(Scene& scene);

	private:
		bool m_ImGuiEditMaterial = false;
	};
}