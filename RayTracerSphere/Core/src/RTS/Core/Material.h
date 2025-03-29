#pragma once

#include "RTS/Common/Base.h"

#include <string>

namespace RTS
{
	struct Material
	{
		Color Albedo{ 1.0f };
		float Roughness = 1.0f;
		float Metallic = 0.0f;
		float EmissionPower = 0.0f; // For emissive materials
		Color EmissionColor{ 1.0f };
		std::string Name = "SampleMaterial";

		bool IsEmissive() const { return EmissionPower > 0.0f; }
		Color GetEmissionColor() const { return EmissionColor * EmissionPower; }

		bool ImGuiProperties();

	private:
		bool m_ImGuiIsEmissive = false;
		bool m_ImGuiEmissiveColor = false;
	};

	class MaterialImGUI
	{
	public:
		MaterialImGUI(Material* material);

		bool ImGuiProperties();
		Material* GetMaterial() { return m_Material; }
	private:
		Material* m_Material;
		bool m_Emission = false;
		bool m_EmissionColorCopyAlbedo = false;
	};
}
