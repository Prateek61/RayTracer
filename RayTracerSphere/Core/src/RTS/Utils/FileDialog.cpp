#include "FileDialog.h"

namespace RTS
{
	void FileDialog::SaveImage(uint32_t width, uint32_t height, Engine::Buffer imageBuffer, const std::filesystem::path& path)
	{
		PROFILE_FUNCTION();

		// Flip the image vertically
		stbi_flip_vertically_on_write(true);
		// Save the image using stb_image_write
		if (stbi_write_png(path.string().c_str(), width, height, 4, imageBuffer.As<void>(), width * 4))
		{
			LOG_TRACE("Saved image: {0}", path.string());
		}
		else
		{
			LOG_ERROR("Failed to save image: {0}", path.string());
		}
	}
}