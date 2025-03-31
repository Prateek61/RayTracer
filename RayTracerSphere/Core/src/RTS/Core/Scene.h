#pragma once

#include "RTS/Common/Base.h"
#include "RTS/Core/Sphere.h"
#include "RTS/Core/Material.h"

#include <vector>
#include <map>

namespace RTS
{
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

		Sphere& GetSphere(uint32_t id);
		Material& GetMaterial(uint32_t id);
		uint32_t GetMaterialID(const std::string& name) const;
	};

	class SceneImGUI
	{
	public:
		SceneImGUI(Scene* scene);

		Scene* GetScene() { return m_Scene; }
		bool ImGuiMaterialSelection(uint32_t& id);

		bool ImGuiMaterialProperties(uint32_t id);
		bool ImGuiSphereProperties(uint32_t id);

		void SceneChanges();

		bool ImGuiProperties();
	private:
		Scene* m_Scene;
		std::vector<MaterialImGUI> m_MaterialImGUIs;
		std::vector<SphereImGUI> m_SphereImGUIs;
		bool m_ViewSpheres = false;
		bool m_ViewMaterials = false;

		bool m_AddSphereDialog = false;
		bool m_AddMaterialDialog = false;

	private:
		bool AddSphereDialog();
		bool AddMaterialDialog();
	};
}