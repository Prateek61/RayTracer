#pragma once

#include "Engine.h"
#include "RayTracerSphere.h"

#include "imgui.h"

class MainLayer : public Engine::Layer
{
public:
	RTS::Material Mat;
	RTS::Scene Scene;
	RTS::Sphere Sphere;
	RTS::MaterialImGUI MatImGUI;
	RTS::SphereImGUI SphereImGUI;
	RTS::SceneImGUI SceneImGUI;

public:
	MainLayer()
		: Layer("MainLayer"), Mat(), Scene(), Sphere(), MatImGUI(&Mat), SphereImGUI(&Sphere, nullptr), SceneImGUI(&Scene)
	{
		Mat.Name = "Another Material";
		Sphere.Name = "I'm just a Sphere";

		Scene.AddMaterial(Mat);
		Mat.Name = "Yet Another Material";
		Scene.AddMaterial(Mat);

		Scene.AddSphere(Sphere);

		MatImGUI = RTS::MaterialImGUI(&Mat);
		SceneImGUI.SceneChanges();
		SphereImGUI = RTS::SphereImGUI(&Sphere, &SceneImGUI);
	}

	void OnUpdate(Engine::TimeStep ts) override
	{
		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Engine::RendererCommand::Clear();
	}

	void OnImGuiRender(Engine::TimeStep ts) override
	{
		ImGui::Begin("Sphere Properties");
		SceneImGUI.ImGuiProperties();
		ImGui::End();
	}
};