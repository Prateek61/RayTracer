#pragma once

#include "Renderer/Shader.h"
#include "Asset/AssetMetadata.h"
#include <filesystem>
#include <set>

namespace Engine
{
	class ShaderMetadata final : public AssetMetadata
	{
	public:
		ShaderMetadata(std::string name, std::filesystem::path vertexPath, std::filesystem::path fragmentPath)
			: AssetMetadata(std::move(name)), VertexPath(std::move(vertexPath)), FragmentPath(std::move(fragmentPath))
		{
		}

		ASSET_TYPE(AssetType::Shader)

		std::filesystem::path VertexPath;
		std::filesystem::path FragmentPath;

		void AddBasePath(const std::filesystem::path& basePath) override
		{
			VertexPath = basePath / VertexPath;
			FragmentPath = basePath / FragmentPath;
		}
	};

	class ComputeShaderMetadata final : public AssetMetadata
	{
	public:
		ComputeShaderMetadata(std::string name, std::filesystem::path computePath)
			: AssetMetadata(std::move(name)), ComputePath(std::move(computePath))
		{
		}

		ASSET_TYPE(AssetType::ComputeShader)

		std::filesystem::path ComputePath;

		void AddBasePath(const std::filesystem::path& basePath) override
		{
			ComputePath = basePath / ComputePath;
		}
	};

	class ShaderLoader
	{
	public:
		static std::shared_ptr<Shader> LoadWithMetadata(const std::shared_ptr<ShaderMetadata>& metadata);
		static std::shared_ptr<Shader> Load(const ShaderMetadata& metadata);
		static std::shared_ptr<Shader> Load(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

		static std::string ParseShader(const std::filesystem::path& path, std::set<std::filesystem::path>& includedFiles);
	};

	class ComputeShaderLoader
	{
	public:
		static std::shared_ptr<ComputeShader> Load(const std::shared_ptr<ComputeShaderMetadata>& metadata);
		static std::shared_ptr<ComputeShader> Load(const std::string& name, const std::filesystem::path& computePath);
	};
}