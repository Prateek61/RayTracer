#pragma once

#include "RayTracer.h"
#include "Engine.h"
#include "Camera.h"
#include "CPURaytracer.h"
#include "CPURaytracer.h"
#include "CPURaytracer.h"
#include "CPURaytracer.h"
#include "Scene.h"

namespace RT
{
	// CPU Raytracer
	class CPURaytracer : public RayTracer
	{
	public:
		CPURaytracer(uint32_t width, uint32_t height)
			: RayTracer(width, height), m_ImageBuffer(width* height * 4), m_Camera{ width, height, glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 1.0f },
			m_Scene()
		{
			InitializeTexture();
		}

		void BindTexture(const uint32_t slot) const override;
		void Render() override;
		float CPURender();
		void SaveImage(const std::filesystem::path& filePath) const;
		void OnImGuiRender();

	private:
		Engine::Buffer m_ImageBuffer; // Float buffer for the image
		std::shared_ptr<Engine::Texture2D> m_Texture;
		Camera m_Camera;
		Scene m_Scene;

	private:
		glm::vec4 RayColor(const Ray& ray);
		void AddToTexture();
		void InitializeTexture();
		void SetPixel(uint32_t x, uint32_t y, const glm::vec4& color);
	};
}
