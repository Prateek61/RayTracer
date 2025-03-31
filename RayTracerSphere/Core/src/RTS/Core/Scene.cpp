#include "Scene.h"
#include "RTS/Core/Material.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace
{
	RTS::Material s_DefaultMaterial{};

	RTS::Material s_NewMaterial{};
	RTS::Sphere s_NewSphere{};
	RTS::MaterialImGUI s_NewMaterialImGUI{ nullptr };
	RTS::SphereImGUI s_NewSphereImGUI{ nullptr, nullptr };
}

namespace RTS
{
	Scene::Scene()
	{
		s_DefaultMaterial.Name = "DefaultMaterial";
		AddMaterial(s_DefaultMaterial);
	}

	uint32_t Scene::AddSphere(const Sphere& sphere)
	{
		Spheres.push_back(sphere);
		return static_cast<uint32_t>(Spheres.size() - 1);
	}

	uint32_t Scene::AddMaterial(const Material& material)
	{
		Materials.push_back(material);
		return static_cast<uint32_t>(Materials.size() - 1);
	}

	Sphere& Scene::GetSphere(uint32_t id)
	{
		return Spheres[id];
	}

	Material& Scene::GetMaterial(uint32_t id)
	{
		return Materials[id];
	}

	uint32_t Scene::GetMaterialID(const std::string& name) const
	{
		for ( uint32_t i = 0; i < Materials.size(); ++i )
		{
			if ( Materials[i].Name == name )
			{
				return i; // +1 to skip the default material
			}
		}
		return 0; // Default material
	}

	SceneImGUI::SceneImGUI(Scene* scene)
		: m_Scene(scene)
	{
		SceneChanges();

		s_NewMaterialImGUI = MaterialImGUI(&s_NewMaterial);
		s_NewSphereImGUI = SphereImGUI(&s_NewSphere, this);
	}

	bool SceneImGUI::ImGuiMaterialSelection(uint32_t& id)
	{
		bool modified = false;

		if ( ImGui::BeginCombo("Select Material", m_MaterialImGUIs[id].GetMaterial()->Name.c_str()) )
		{
			for ( uint32_t i = 0; i < m_MaterialImGUIs.size(); ++i )
			{
				bool selected = id == i;
				if ( ImGui::Selectable(m_MaterialImGUIs[i].GetMaterial()->Name.c_str(), selected) )
				{
					id = i;
					modified = true;
				}
				if ( selected )
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		return modified;
	}

	bool SceneImGUI::ImGuiMaterialProperties(uint32_t id)
	{
		return m_MaterialImGUIs[id].ImGuiProperties();
	}

	bool SceneImGUI::ImGuiSphereProperties(uint32_t id)
	{
		return m_SphereImGUIs[id].ImGuiProperties();
	}

	void SceneImGUI::SceneChanges()
	{
		// Clear the vectors
		m_MaterialImGUIs.clear();
		m_SphereImGUIs.clear();

		for ( Sphere& sphere : m_Scene->Spheres )
		{
			m_SphereImGUIs.emplace_back(&sphere, this);
		}
		for ( Material& material : m_Scene->Materials )
		{
			m_MaterialImGUIs.emplace_back(&material);
		}
	}

	bool SceneImGUI::ImGuiProperties()
	{
		bool modified = false;

		// Button to add a new sphere
		bool add_sphere = ImGui::Button("Add Sphere");
		ImGui::SameLine();
		bool add_material = ImGui::Button("Add Material");
		m_AddSphereDialog |= add_sphere;
		m_AddMaterialDialog |= add_material;

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("##SceneViewSpheres", &m_ViewSpheres);
		ImGui::SameLine();
		ImGui::Text("View Spheres");

		if ( m_ViewSpheres )
		{
			for ( uint32_t i = 0; i < m_SphereImGUIs.size(); ++i )
			{
				ImGui::PushID(static_cast<int>(i));

				bool open = ImGui::CollapsingHeader("##SceneSphereCollapsingHeader");
				ImGui::SameLine();
				ImGui::Text("%s", m_SphereImGUIs[i].GetSphere()->Name.c_str());

				if ( open )
				{
					modified |= m_SphereImGUIs[i].ImGuiProperties();
				}

				ImGui::PopID();
				ImGui::Spacing();
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("##SceneViewMaterials", &m_ViewMaterials);
		ImGui::SameLine();
		ImGui::Text("View Materials");

		uint32_t total_spheres = static_cast<uint32_t>(m_Scene->Spheres.size());
		if ( m_ViewMaterials )
		{
			for ( uint32_t i = 0; i < m_MaterialImGUIs.size(); ++i )
			{
				ImGui::PushID(static_cast<int>(total_spheres + i));
				bool open = ImGui::CollapsingHeader("##SceneMaterialCollapsingHeader");
				ImGui::SameLine();
				ImGui::Text("%s", m_MaterialImGUIs[i].GetMaterial()->Name.c_str());
				if ( open )
				{
					ImGui::Spacing();
					modified |= m_MaterialImGUIs[i].ImGuiProperties();
					ImGui::Spacing();
					ImGui::Separator();
				}
				ImGui::PopID();

				ImGui::Spacing();
			}
		}

		ImGui::PushID("Add Dialog");
		if (m_AddSphereDialog)
		{
			if (ImGui::Begin("Add Sphere", &m_AddSphereDialog))
			{
				modified |= AddSphereDialog();
			}
			else
			{
				s_NewSphere = {};
			}
			ImGui::End();
		}
		if (m_AddMaterialDialog)
		{
			if (ImGui::Begin("Add Material", &m_AddMaterialDialog))
			{
				modified |= AddMaterialDialog();
			}
			else
			{
				s_NewMaterial = {};
			}
			ImGui::End();
		}
		ImGui::PopID();

		return modified;
	}

	bool SceneImGUI::AddSphereDialog()
	{
		bool modified = false;

		s_NewSphereImGUI.ImGuiProperties();

		if ( ImGui::Button("Add Sphere"))
		{
			uint32_t id = m_Scene->AddSphere(s_NewSphere);
			SceneChanges();
			m_AddSphereDialog = false;
			s_NewSphere = {};
			modified = true;
		}

		return modified;
	}
	bool SceneImGUI::AddMaterialDialog()
	{
		bool modified = false;

		s_NewMaterialImGUI.ImGuiProperties();

		if (ImGui::Button("Add Material"))
		{
			uint32_t id = m_Scene->AddMaterial(s_NewMaterial);
			SceneChanges();
			m_AddMaterialDialog = false;
			s_NewMaterial = {};
			modified = true;
		}

		return modified;
	}
}
