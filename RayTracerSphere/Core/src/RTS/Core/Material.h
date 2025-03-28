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
		Color EmissionColor{ 0.0f };
		std::string Name = "SampleMaterial";

		bool IsEmissive() const { return EmissionPower > 0.0f; }
		Color GetEmissionColor() const { return EmissionColor * EmissionPower; }

		bool ImGuiProperties();

	private:
		bool m_ImGuiIsEmissive = false;
		bool m_ImGuiEmissiveColor = false;
	};
}
