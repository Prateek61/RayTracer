#pragma once

#include "Engine.h"
#include "RayTracerSphere.h"

#include "imgui.h"

class MainLayer : public Engine::Layer
{
public:
	RTS::Material Mat;

public:
	MainLayer()
		: Layer("MainLayer"), Mat()
	{
	}

	void OnUpdate(Engine::TimeStep ts) override
	{
		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Engine::RendererCommand::Clear();
	}

	void OnImGuiRender(Engine::TimeStep ts) override
	{
		ImGui::Begin("Material Properties");

		Mat.ImGuiProperties();

		ImGui::End();
	}
};