#pragma once

#include "RTS/CPU/SceneInteraction.h"

#include "RTS/Core/RayTracer.h"
#include "RTS/Core/Camera.h"
#include "RTS/Core/Scene.h"

namespace RTS
{
	class CPURayTracer final : public RayTracer
	{
	public:
		CPURayTracer(Camera* camera, Scene* scene, uint32_t width, uint32_t height);
		~CPURayTracer() override;

		void Update(RenderTargetType type = RenderTargetType::None) override;
		void BindTexture(uint32_t slot, RenderTargetType type = RenderTargetType::None) override;
		void OnImGUIRender() override;
		void OnResize(uint32_t width, uint32_t height) override;
		void OnSceneOrCameraChange() override;
		Engine::Buffer GetImageBuffer(RenderTargetType type) override;

	private:
		Camera* m_Camera;
		Scene* m_Scene;
		SceneInteraction m_SceneInteraction;
		uint32_t m_Width, m_Height;

	private:
		// float buffers for internal calculations
		Engine::Buffer m_AccumulationBuffer{};
		Engine::Buffer m_DepthBuffer{};
		Engine::Buffer m_BaseColorBuffer{};
		// 8-bit image buffers
		Engine::Buffer m_RenderImageBuffer{};
		Engine::Buffer m_DepthImageBuffer{};
		Engine::Buffer m_BaseColorImageBuffer{};
		// Textures
		std::shared_ptr<Engine::Texture2D> m_RenderTexture;
		std::shared_ptr<Engine::Texture2D> m_DepthTexture;
		std::shared_ptr<Engine::Texture2D> m_BaseColorTexture;

		// Flags
		RenderTargetType m_ActiveRenderType = RenderTargetType::Render;
		uint32_t m_AccumulationCount = 0;
		int m_Bounces = 10;
		float m_LastAccumulationTime = 0.0f;
		bool m_BaseColorRendered = false;
		bool m_DepthRendered = false;
		bool m_Accumulate = false;

	private:
		void ResizeBuffersAndTextures();

		void ResetFloatBuffer(Engine::Buffer& buffer) const;
		void ResetImageBuffer(Engine::Buffer& buffer) const;
		void AddToTexture(Engine::Buffer& buffer, std::shared_ptr<Engine::Texture2D>& texture);
		void ToImageBuffer(Engine::Buffer& buffer, Engine::Buffer& imageBuffer);

		void Render();
		void RenderDepth();
		void RenderBaseColor();

		void Accumulate();
		void ComputeDepth();
		void ComputeBaseColor();
	};
}