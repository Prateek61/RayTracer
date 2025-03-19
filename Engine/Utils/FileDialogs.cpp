#include "PCH.h"
#include "FileDialogs.h"

namespace Engine
{
	std::string FileDialogs::ReadFileStr(const std::filesystem::path& filepath)
	{
		PROFILE_FUNCTION();

		// Check if the file exists
		if (!std::filesystem::exists(filepath))
		{
			LOG_CORE_ERROR("FileIO::RealFileStr: File does not exist: {0}", filepath.string());
			return {};
		}

		std::ifstream stream(filepath, std::ios::binary | std::ios::in);
		if (!stream.is_open())
		{
			LOG_CORE_ERROR("FileIO::RealFileStr: Could not open file: {0}", filepath.string());
			return {};
		}

		stream.seekg(0, std::ios::end);
		const uint64_t size = stream.tellg();
		if (size == 0)
		{
			LOG_CORE_WARN("FileIO::RealFileStr: File is empty: {0}", filepath.string());
			return {};
		}

		std::string result;
		result.resize(size);
		stream.seekg(0, std::ios::beg);
		stream.read(result.data(), static_cast<int64_t>(size));

		return result;
	}

	bool FileDialogs::WriteFileStr(const std::filesystem::path& filepath, const std::string& content)
	{
		PROFILE_FUNCTION();

		// Create the directory if it does not exist
		if (!std::filesystem::exists(filepath.parent_path()))
		{
			std::filesystem::create_directories(filepath.parent_path());
		}

		// Open the file
		std::ofstream stream(filepath, std::ios::binary | std::ios::out);
		if (!stream.is_open())
		{
			LOG_CORE_ERROR("FileIO::WriteFileStr: Could not open file: {0}", filepath.string());
			return false;
		}

		// Write the content
		stream.write(content.data(), content.size());

		return true;
	}

}