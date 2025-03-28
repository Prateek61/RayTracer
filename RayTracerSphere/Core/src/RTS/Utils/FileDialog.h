#pragma once

#include "Engine.h"

#include "stb_image_write.h"

#include <cstdint>
#include <filesystem>

namespace RTS
{
	class FileDialog
	{
	public:
		static void SaveImage(uint32_t width, uint32_t height, Engine::Buffer imageBuffer, const std::filesystem::path& path);
	};
}