#include "Material.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace RTS
{
	MaterialImGUI::MaterialImGUI(Material* material)
		: m_Material(material)
	{
		if (m_Material)
			m_Emission |= m_Material->IsEmissive();
	}

	bool MaterialImGUI::ImGuiProperties()
	{
		bool modified = false;

		m_Emission |= m_Material->IsEmissive();

		// Show the name of the material
		modified |= ImGui::InputText("##MaterialName", &m_Material->Name);
		ImGui::SameLine();
		ImGui::Text("Name");

		ImGui::Spacing();

		// Albedo
		modified = ImGui::ColorEdit3("##MaterialAlbedo", &m_Material->Albedo.r);
		ImGui::SameLine();
		ImGui::Text("Albedo");

		ImGui::Spacing();

		// Roughness
		modified |= ImGui::SliderFloat("##MaterialRoughness", &m_Material->Roughness, 0.0f, 1.0f);
		ImGui::SameLine();
		ImGui::Text("Roughness");

		ImGui::Spacing();

		// Metallic
		modified |= ImGui::SliderFloat("##MaterialMetallic", &m_Material->Metallic, 0.0f, 1.0f);
		ImGui::SameLine();
		ImGui::Text("Metallic");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Checkbox for Emissive material
		if (ImGui::Checkbox("##MaterialEmissive", &m_Emission))
		{
			modified = true;
			if (!m_Emission)
			{
				m_Material->EmissionPower = 0.0f;
				m_Material->EmissionColor = Color{ 1.0f };
			}
		}
		ImGui::SameLine();
		ImGui::Text("Emissive");

		if (m_Emission)
		{
			ImGui::Spacing();

			// Emission Power
			modified |= ImGui::DragFloat("##MaterialEmissionPower", &m_Material->EmissionPower, 0.1f, 0.0f, 100.0f);
			ImGui::SameLine();
			ImGui::Text("Emission Power");

			ImGui::Spacing();

			// Emission Color
			modified |= ImGui::ColorEdit3("##MaterialEmissionColor", &m_Material->EmissionColor.r);
			ImGui::SameLine();
			ImGui::Text("Emission Color");

			ImGui::Spacing();

			// Emission Color same as Albedo
			modified |= ImGui::Checkbox("##MaterialEmissionColorSameAsAlbedo", &m_EmissionColorCopyAlbedo);
			ImGui::SameLine();
			ImGui::Text("Emission Color Same as Albedo");
			if (m_EmissionColorCopyAlbedo)
			{
				m_Material->EmissionColor = m_Material->Albedo;
			}
		}

		return modified;
	}
}