#include "Utils.h"
#include "stb_image_write.h"

namespace RT::Utils
{
	uint32_t Image::ConvertToPixel(float r, float g, float b, float a)
	{
		const uint32_t red = static_cast<uint32_t>(r * 255.999f);
		const uint32_t green = static_cast<uint32_t>(g * 255.999f);
		const uint32_t blue = static_cast<uint32_t>(b * 255.999f);
		const uint32_t alpha = static_cast<uint32_t>(a * 255.999f);
		const uint32_t pixel = alpha << 24 | red << 16 | green << 8 | blue;
		return pixel;
	}

	uint32_t Image::ConvertToPixel(const glm::vec4& color)
	{
		const uint32_t red = static_cast<uint32_t>(color.r * 255.999f);
		const uint32_t green = static_cast<uint32_t>(color.g * 255.999f);
		const uint32_t blue = static_cast<uint32_t>(color.b * 255.999f);
		const uint32_t alpha = static_cast<uint32_t>(color.a * 255.999f);
		const uint32_t pixel = alpha << 24 | red << 16 | green << 8 | blue;
		return pixel;
	}

	void Image::SaveImage(uint32_t width, uint32_t height, Engine::Buffer imageBuffer, const std::filesystem::path& filePath)
	{
		// Flip the image vertically
		stbi_flip_vertically_on_write(true);
		// Save the image using stb_image_write
		if (stbi_write_png(filePath.string().c_str(), width, height, 4, imageBuffer.As<void>(), width * 4))
		{
			LOG_TRACE("Saved image: {0}", filePath.string());
		}
		else
		{
			LOG_ERROR("Failed to save image: {0}", filePath.string());
		}
	}
}