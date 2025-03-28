#pragma once

#include "RTS/Common/Base.h"
#include "RTS/Core/Sphere.h"
#include "RTS/Core/Material.h"

#include <vector>

namespace RTS
{
	class Scene
	{
	public:
		std::vector<Sphere> Spheres;
		std::vector<Material> Materials;

	public:
		uint32_t AddSphere(const Sphere& sphere)
		{
			Spheres.push_back(sphere);
			return static_cast<uint32_t>(Spheres.size() - 1);
		}

		uint32_t AddMaterial(const Material& material)
		{
			Materials.push_back(material);
			return static_cast<uint32_t>(Materials.size() - 1);
		}
	};
}