#pragma once

#include "RayTracer.h"
#include "Engine.h"

namespace RT
{
	// CPU Raytracer
	class CPURaytracer : public RayTracer
	{
	public:
		CPURaytracer(uint32_t width, uint32_t height)
			: RayTracer(width, height), m_ImageBuffer(width * height * 4)
		{
			InitializeTexture();
		}

		void BindTexture(const uint32_t slot) const override;
		void Render() override;
		float CPURender();
		void SaveImage(const std::filesystem::path& filePath) const;

	private:
		Engine::Buffer m_ImageBuffer; // Float buffer for the image
		std::shared_ptr<Engine::Texture2D> m_Texture;

	private:
		void AddToTexture();
		void InitializeTexture();
		void SetPixel(uint32_t x, uint32_t y, float r, float g, float b, float a = 1.0f);
	};
}