#pragma once

#include "RTS/Common/Base.h"

#include <string>
#include <cstdint>
#include <vector>

namespace RTS
{
	class SceneImGUI;

	struct Sphere
	{
		Point Center{ 0.0f };
		float Radius{ 1.0f };
		uint32_t MaterialID{ 0 };
		std::string Name = "SampleSphere";
	};

	class SphereImGUI
	{
	public:
		SphereImGUI(Sphere* sphere, SceneImGUI* sceneImGUI);


		bool ImGuiProperties();
		Sphere* GetSphere() { return m_Sphere; }

	private:
		Sphere* m_Sphere;
		SceneImGUI* m_SceneImGUI;
		bool m_ImGuiEditMaterial = false;
	};
}