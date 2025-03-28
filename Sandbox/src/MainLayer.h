#pragma once

#include "Engine.h"
#include "ViewportTexture.h"
#include "RayTracer/CPURaytracer.h"


class MainLayer : public Engine::Layer
{
public:
	MainLayer(uint32_t m_Width, uint32_t m_Height, const std::filesystem::path& assetDir)
		: Layer("ExampleLayer"), m_Width(m_Width), m_Height(m_Height), m_CPURaytracer(m_Width, m_Height), m_AssetDir(assetDir)
	{
	}
	~MainLayer() override = default;

	void OnAttach() override;
	void OnUpdate(Engine::TimeStep ts) override;
	void OnImGuiRender(Engine::TimeStep ts) override;

private:
	uint32_t m_Width, m_Height;
	RT::CPURaytracer m_CPURaytracer;
	std::unique_ptr<ViewportTexture> m_ViewportTexture;
	float m_RenderTime = 0.0f;
	std::string m_Filename = "Render";
	std::filesystem::path m_AssetDir;

private:
	void RenderImage();
};
