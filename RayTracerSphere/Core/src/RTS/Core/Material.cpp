#include "Material.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace RTS
{
	bool Material::ImGuiProperties()
	{
		bool modified = false;

		// Show the name of the material
		modified |= ImGui::InputText("##Name", &Name);
		ImGui::SameLine();
		ImGui::Text("Name");

		ImGui::Spacing();
		modified |= ImGui::ColorEdit3("##Albedo", &Albedo.r);
		ImGui::SameLine();
		ImGui::Text("Albedo");

		ImGui::Spacing();
		modified |= ImGui::SliderFloat("##Roughness", &Roughness, 0.0f, 1.0f);
		ImGui::SameLine();
		ImGui::Text("Roughness");

		ImGui::Spacing();
		modified |= ImGui::SliderFloat("##Metallic", &Metallic, 0.0f, 1.0f);
		ImGui::SameLine();
		ImGui::Text("Metallic");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Checkbox for emissive material
		if (ImGui::Checkbox("Emissive", &m_ImGuiIsEmissive))
		{
			modified = true;
			if (!m_ImGuiIsEmissive)
			{
				EmissionPower = 0.0f;
				EmissionColor = Color{ 0.0f };
			}
		}

		if (m_ImGuiIsEmissive)
		{
			ImGui::Spacing();
			modified |= ImGui::SliderFloat("##EmissionPower", &EmissionPower, 0.0f, 100.0f);
			ImGui::SameLine();
			ImGui::Text("Emission Power");

			ImGui::Spacing();
			modified |= ImGui::ColorEdit3("##EmissionColor", &EmissionColor.r);
			ImGui::SameLine();
			ImGui::Text("Emission Color");

			ImGui::Spacing();
			modified |= ImGui::Checkbox("Emission Color Same as Albedo", &m_ImGuiEmissiveColor);

			if (m_ImGuiEmissiveColor)
			{
				EmissionColor = Albedo;
			}
		}

		return modified;
	}
}