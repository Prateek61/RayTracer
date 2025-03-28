#pragma once

#include "RTS/Common/Base.h"
#include "RTS/Core/Sphere.h"

#include <vector>
#include <map>

namespace RTS
{
	struct Material;

	class Scene
	{
	public:
		std::vector<Sphere> Spheres;
		std::vector<Material> Materials;

	public:
		Scene();

		uint32_t AddSphere(const Sphere& sphere);
		uint32_t AddMaterial(const Material& material);

		uint32_t GetMaterialCount() const { return static_cast<uint32_t>(Materials.size()); }

		Material& GetMaterial(uint32_t id);
		std::vector<std::string> GetMaterialNames() const;
		uint32_t GetMaterialID(const std::string& name) const;
	}; 
}