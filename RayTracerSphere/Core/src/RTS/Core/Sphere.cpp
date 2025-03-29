#include "Sphere.h"
#include "RTS/Core/Scene.h"

#include "imgui.h"
#include "Material.h"

#include "misc/cpp/imgui_stdlib.h"

namespace
{
}

namespace RTS
{
	SphereImGUI::SphereImGUI(Sphere* sphere, SceneImGUI* sceneImGUI)
		: m_Sphere(sphere), m_SceneImGUI(sceneImGUI)
	{
	}
	bool SphereImGUI::ImGuiProperties()
	{
		bool modified = false;

		// Name
		modified |= ImGui::InputText("##SphereName", &m_Sphere->Name);
		ImGui::SameLine();
		ImGui::Text("Name");

		ImGui::Spacing();

		// Center
		modified |= ImGui::DragFloat3("##SphereCenter", &m_Sphere->Center.x, 0.1f);
		ImGui::SameLine();
		ImGui::Text("Center");

		ImGui::Spacing();

		// Radius
		modified |= ImGui::DragFloat("##SphereRadius", &m_Sphere->Radius, 0.1f);
		ImGui::SameLine();
		ImGui::Text("Radius");

		ImGui::Separator();

		// Material
		modified |= m_SceneImGUI->ImGuiMaterialSelection(m_Sphere->MaterialID);

		// View Material
		ImGui::Checkbox("##Sphere-ViewMaterial", &m_ImGuiEditMaterial);
		ImGui::SameLine();
		ImGui::Text("View Material");

		if (m_ImGuiEditMaterial)
		{
			modified |= m_SceneImGUI->ImGuiMaterialProperties(m_Sphere->MaterialID);
		}

		return modified;
	}
}
