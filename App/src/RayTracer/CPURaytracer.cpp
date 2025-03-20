#include "CPURaytracer.h"
#include "RayTracer/Utils.h"

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

				SetPixel(x, y, r, g, b);
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

	void CPURaytracer::AddToTexture()
	{
		m_Texture->SetData(m_ImageBuffer);
	}

	void CPURaytracer::InitializeTexture()
	{
		Engine::TextureSpecification spec(m_Width, m_Height);
		m_Texture = std::make_shared<Engine::Texture2D>(spec);
	}

	void CPURaytracer::SetPixel(uint32_t x, uint32_t y, float r, float g, float b, float a)
	{
		uint32_t* data = m_ImageBuffer.As<uint32_t>();
		data[y * m_Width + x] = Utils::Image::ConvertToPixel(r, g, b, a);
	}
}
