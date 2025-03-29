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
		ImGui::Text("Material");

		ImGui::Spacing();

		// Metallic
		modified |= ImGui::SliderFloat("##MaterialMetallic", &m_Material->Metallic, 0.0f, 1.0f);
		ImGui::SameLine();
		ImGui::Text("Material");

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