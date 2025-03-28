#include "Scene.h"
#include "RTS/Core/Material.h"

namespace
{
	RTS::Material s_DefaultMaterial{};
}

namespace RTS
{
	Scene::Scene()
	{
		s_DefaultMaterial.Name = "DefaultMaterial";
	}

	uint32_t Scene::AddSphere(const Sphere& sphere)
	{
		Spheres.emplace_back(sphere);
		return static_cast<uint32_t>(Spheres.size() - 1);
	}

	uint32_t Scene::AddMaterial(const Material& material)
	{
		Materials.emplace_back(material);
		return static_cast<uint32_t>(Materials.size() - 1);
	}

	Material& Scene::GetMaterial(uint32_t id)
	{
		if (id <= Materials.size() && id > 0)
			return Materials[id - 1];
		
		return s_DefaultMaterial;
	}

	std::vector<std::string> Scene::GetMaterialNames() const
	{
		std::vector<std::string> names;
		names.reserve(Materials.size() + 1);
		names.push_back(s_DefaultMaterial.Name);
		for (const auto& material : Materials)
		{
			names.push_back(material.Name);
		}
		return names;
	}

	uint32_t Scene::GetMaterialID(const std::string& name) const
	{
		for (uint32_t i = 0; i < Materials.size(); ++i)
		{
			if (Materials[i].Name == name)
			{
				return i + 1; // +1 to skip the default material
			}
		}
		return 0; // Default material
	}
}
