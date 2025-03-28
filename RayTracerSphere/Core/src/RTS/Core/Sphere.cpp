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
	bool Sphere::ImGuiProperties(Scene& scene)
	{
		bool modified = false;

		// Show the name of the sphere
		modified |= ImGui::InputText("Name", &Name);

		// Center
		modified |= ImGui::DragFloat3("Center", &Center.x, 0.1f);
		// Radius
		modified |= ImGui::DragFloat("Radius", &Radius, 0.1f);

		ImGui::Separator();

		// Materials
		// Checkbox to edit material
		if ( ImGui::BeginCombo("Select Material", scene.GetMaterial(MaterialID).Name.c_str()) )
		{
			// Get the list of material names
			auto material_names = scene.GetMaterialNames();
			for ( uint32_t i = 0; i < material_names.size(); ++i )
			{
				bool selected = MaterialID == i;
				if ( ImGui::Selectable(material_names[i].c_str(), selected) )
				{
					MaterialID = i;
					modified = true;
				}
				if ( selected )
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		// View Material
		ImGui::Checkbox("View Material", &m_ImGuiEditMaterial);
		if (m_ImGuiEditMaterial)
		{

			modified |= scene.GetMaterial(MaterialID).ImGuiProperties();
		}

		return modified;
	}
}
