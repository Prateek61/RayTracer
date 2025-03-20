#include "CPURaytracer.h"
#include "RayTracer/Utils.h"
#include "imgui.h"

namespace RT
{
	void CPURaytracer::BindTexture(const uint32_t slot) const
	{
		if ( m_Texture )
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

		uint32_t* data = m_ImageBuffer.As<uint32_t>();
		for ( uint32_t y = 0; y < m_Height; y++ )
		{
			for ( uint32_t x = 0; x < m_Width; x++ )
			{
				float r = static_cast<float>(x) / static_cast<float>(m_Width);
				float g = static_cast<float>(y) / static_cast<float>(m_Height);
				float b = 0.2f;

				auto ray = m_Camera.GetRay(x, y);
				SetPixel(x, y, RayColor(ray));
			}
		}

		float time = timer.ElapsedMillis();

		// Add the int buffer to the texture
		AddToTexture();

		return time;
	}

	void CPURaytracer::SaveImage(const std::filesystem::path& filePath) const
	{
		Utils::Image::SaveImage(m_Width, m_Height, m_ImageBuffer, filePath);
	}

	void CPURaytracer::OnImGuiRender()
	{
		// Mark this section as camera
		ImGui::Text("Camera");
		m_Camera.OnImGuiRender();
	}

	glm::vec4 CPURaytracer::RayColor(const Ray& ray)
	{
		glm::vec3 color = m_Scene.RayColor(ray);
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

	void CPURaytracer::SetPixel(uint32_t x, uint32_t y, const glm::vec4& color)
	{
		uint32_t* data = m_ImageBuffer.As<uint32_t>();
		uint32_t pixel = Utils::Image::ConvertToPixel(color);
		// LOG PIXEL as HEX
		//LOG_TRACE("Pixel: {0:x}", pixel);
		data[y * m_Width + x] = pixel;
	}
}
