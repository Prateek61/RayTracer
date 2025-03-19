#include "PCH.h"
#include "ShaderLoader.h"

#include "Utils/FileDialogs.h"

#include <regex>

namespace Engine
{
	std::shared_ptr<Shader> ShaderLoader::LoadWithMetadata(const std::shared_ptr<ShaderMetadata>& metadata)
	{
		return Load(metadata->Name, metadata->VertexPath, metadata->FragmentPath);
	}

	std::shared_ptr<Shader> ShaderLoader::Load(const ShaderMetadata& metadata)
	{
		return Load(metadata.Name, metadata.VertexPath, metadata.FragmentPath);
	}

	std::shared_ptr<Shader> ShaderLoader::Load(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		PROFILE_FUNCTION();

		std::set<std::filesystem::path> included_files;
		std::string vertex_source = ParseShader(vertexPath, included_files);

		if (vertex_source.empty())
		{
			throw AssetLoadingError(AssetType::Shader, "Failed to load vertex shader (" + name + ") source from file: " + vertexPath.string());
			return nullptr;
		}

		included_files.clear();
		std::string fragment_source = ParseShader(fragmentPath, included_files);
		if (fragment_source.empty())
		{
			throw AssetLoadingError(AssetType::Shader, "Failed to load fragment shader (" + name + ") source from file: " + fragmentPath.string());
			return nullptr;
		}

		std::shared_ptr<Shader> shader;

		try
		{
			shader = std::make_shared<Shader>(name, vertex_source, fragment_source);
		}
		catch ( ShaderCompilationError& error)
		{
			throw AssetLoadingError(AssetType::Shader, "Failed to compile shader (" + name + "): " + error.what());
			return nullptr;
		}

		return shader;
	}

	std::string ShaderLoader::ParseShader(const std::filesystem::path& path, std::set<std::filesystem::path>& includedFiles)
	{
		PROFILE_FUNCTION();

		bool is_main = includedFiles.empty();
		const std::filesystem::path directory = path.parent_path();
		const std::filesystem::path filename = path.filename();
		const std::filesystem::path current_file_path = directory / filename;

		std::string result = FileDialogs::ReadFileStr(current_file_path);

		// Check if the file is empty
		if (result.empty())
		{
			return {};
		}

		// Look for #include "file" in the shader source and replace it with the contents of the file
		std::regex include_regex(R"(#include\s+\"([^\"]+)\")");
		std::smatch match;
		std::string processed_result = result;

		while (std::regex_search(processed_result, match, include_regex))
		{
			std::filesystem::path include_filepath = directory / match[1].str();

			// Check if the file is already included
			if (includedFiles.contains(include_filepath))
			{
				processed_result.replace(match.position(), match.length(), "");
				continue;
			}

			// Add the included file to the set to prevent circular includes
			includedFiles.insert(include_filepath);

			std::string include_result = ParseShader(include_filepath, includedFiles);
			processed_result.replace(match.position(), match.length(), include_result);
		}

		// Save the result to file_name + ".processed" if this is the main file
		if (is_main)
		{
			const std::filesystem::path processed_path = directory / (filename.string() + ".processed");
			FileDialogs::WriteFileStr(processed_path, processed_result);
		}

		return processed_result;
	}

	std::shared_ptr<ComputeShader> ComputeShaderLoader::Load(const std::shared_ptr<ComputeShaderMetadata>& metadata)
	{
		return Load(metadata->Name, metadata->ComputePath);
	}

	std::shared_ptr<ComputeShader> ComputeShaderLoader::Load(const std::string& name, const std::filesystem::path& computePath)
	{
		PROFILE_FUNCTION();

		std::set<std::filesystem::path> included_files;
		std::string compute_source = ShaderLoader::ParseShader(computePath, included_files);

		if (compute_source.empty())
		{
			throw AssetLoadingError(AssetType::ComputeShader, "Failed to load compute shader (" + name + ") source from file: " + computePath.string());
			return nullptr;
		}

		std::shared_ptr<ComputeShader> shader;
		try
		{
			shader = std::make_shared<ComputeShader>(name, compute_source);
		}
		catch (ShaderCompilationError& error)
		{
			throw AssetLoadingError(AssetType::ComputeShader, "Failed to compile compute shader (" + name + "): " + error.what());
			return nullptr;
		}

		return shader;
	}
}