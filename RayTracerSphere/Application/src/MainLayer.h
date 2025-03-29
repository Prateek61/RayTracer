#pragma once

#include "Engine.h"
#include "RayTracerSphere.h"
#include "ViewportTexture.h"

class MainLayer : public Engine::Layer
{
public:
	MainLayer(uint32_t width, uint32_t height, std::filesystem::path assetPath);

	void OnUpdate(Engine::TimeStep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender(Engine::TimeStep ts) override;
private:
	RTS::Scene m_Scene;
	RTS::Camera m_Camera;
	RTS::CPURayTracer m_CPURayTracer;
	std::filesystem::path m_AssetPath;
	std::unique_ptr<ViewportTexture> m_ViewportTexture;

	// ImGUI
	RTS::SceneImGUI m_SceneImGUI;

	bool m_CameraOrSceneChanged = true;

private:
	void InitializeScene();
};
