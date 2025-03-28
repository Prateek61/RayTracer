#include "CPURaytracer.h"
#include "RayTracer/Utils.h"
#include "imgui.h"
#include <omp.h>

namespace RT
{
	void CPURaytracer::BindTexture(const uint32_t slot) const
	{
		if (m_Texture)
		{
			m_Texture->Bind(slot);
		}
	}

	void CPURaytracer::Render()
	{
		float _ = CPURender();
	}

	float CPURaytracer::CPURender()
	{
		Engine::Timer timer;

		if (m_Camera.Changed)
		{
			m_Camera.Changed = false;
			ClearAccumulation();
		}

		if (m_AccumulationCount == 0 || m_Accumulate)
		{
			Accumulate();
			ConvertAccumulationToImage();
			AddToTexture();
		}

		return timer.ElapsedMillis();
	}

	void CPURaytracer::SaveImage(const std::filesystem::path& filePath) const
	{
		Utils::Image::SaveImage(m_Width, m_Height, m_ImageBuffer, filePath);
	}

	void CPURaytracer::OnImGuiRender()
	{
		// Check box to toggle accumulation
		ImGui::Checkbox("Accumulate", &m_Accumulate);
		// Button to clear accumulation
		if (ImGui::Button("Clear Accumulation"))
		{
			ClearAccumulation();
		}
		// Show accumulation count
		ImGui::Text("Accumulation Count: %d", m_AccumulationCount);

		// Slider for max depth
		ImGui::SliderInt("Max Depth", &m_MaxDepth, 1, 100);

		if (ImGui::Checkbox("Gamma Correction", &m_GammaCorrection))
		{
			if (!m_Accumulate)
			{
				ConvertAccumulationToImage();
				AddToTexture();
			}
		}

		static bool camera_options = false;
		// Add a checkbox to toggle the Camera Options
		ImGui::Checkbox("Camera Options", &camera_options);

		if (camera_options)
		{
			ImGui::Begin("Camera Options");
			m_Camera.OnImGuiRender();
			ImGui::End();
		}
	}

	void CPURaytracer::ClearAccumulation()
	{

		glm::vec4* data = m_AccumulationBuffer.As<glm::vec4>();
		for (uint32_t y = 0; y < m_Height; y++)
		{
			for (uint32_t x = 0; x < m_Width; x++)
			{
				data[y * m_Width + x] = { 0.0f, 0.0f, 0.0f, 0.0f };
			}
		}

		m_AccumulationCount = 0;
	}

	glm::vec4 CPURaytracer::RayColor(const Ray& ray)
	{
		glm::vec3 color = m_Scene.RayColor(ray, m_MaxDepth);
		return { color, 1.0f };
	}

	void CPURaytracer::AddToTexture()
	{
		m_Texture->SetData(m_ImageBuffer);
	}

	void CPURaytracer::InitializeTexture()
	{
		Engine::TextureSpecification spec(m_Width, m_Height);
		m_Texture = std::make_shared<Engine::Texture2D>(spec);
	}

	void CPURaytracer::ConvertAccumulationToImage()
	{
		glm::vec4* accumulation_data = m_AccumulationBuffer.As<glm::vec4>();
		uint32_t* image_data = m_ImageBuffer.As<uint32_t>();
		for (uint32_t y = 0; y < m_Height; y++)
		{
			for (uint32_t x = 0; x < m_Width; x++)
			{
				glm::vec4 color = accumulation_data[y * m_Width + x] / static_cast<float>(m_AccumulationCount);

				if (m_GammaCorrection)
				{
					color = Utils::Image::LinearToGamma(color);
				}

				image_data[y * m_Width + x] = Utils::Image::ConvertToPixel(color);
			}
		}
	}

	void CPURaytracer::Accumulate()
	{
		glm::vec4* data = m_AccumulationBuffer.As<glm::vec4>();
		#pragma omp parallel for
		for (int y = 0; y < (int)m_Height; y++)
		{
			for (uint32_t x = 0; x < m_Width; x++)
			{
				auto ray = m_Camera.GetRay(x, y);
				data[y * m_Width + x] += RayColor(ray);
			}
		}
		m_AccumulationCount++;
	}

	void CPURaytracer::SetPixel(uint32_t x, uint32_t y, const glm::vec4& color)
	{
		uint32_t* data = m_ImageBuffer.As<uint32_t>();
		uint32_t pixel = Utils::Image::ConvertToPixel(color);
		// LOG PIXEL as HEX
		//LOG_TRACE("Pixel: {0:x}", pixel);
		data[y * m_Width + x] = pixel;
	}
}
