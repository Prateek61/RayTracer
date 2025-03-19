#pragma once

#include "Engine.h"
#include "ViewportTexture.h"


class MainLayer : public Engine::Layer
{
public:
	MainLayer(uint32_t m_Width, uint32_t m_Height)
		: Layer("ExampleLayer"), m_Width(m_Width), m_Height(m_Height), m_ImageBuffer(m_Width* m_Height * 4)
	{
	}
	~MainLayer() override = default;

	void OnAttach() override;
	void OnUpdate(Engine::TimeStep ts) override;
	void OnImGuiRender(Engine::TimeStep ts) override;

private:
	std::unique_ptr<ViewportTexture> m_ViewportTexture;
	std::shared_ptr<Engine::ComputeShader> m_ComputeShader;
	uint32_t m_Width, m_Height;
	Engine::Buffer m_ImageBuffer;
	float m_RenderTime = 0.0f;

private:
	void RenderImage();
};
