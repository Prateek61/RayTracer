#pragma once

#include <filesystem>
#include <string>

namespace Engine
{
	class FileDialogs
	{
	public:
		// Reads a file and returns the content as a string
		static std::string ReadFileStr(const std::filesystem::path& filepath);
		static bool WriteFileStr(const std::filesystem::path& filepath, const std::string& content);
	};
}