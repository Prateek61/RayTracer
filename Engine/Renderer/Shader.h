#pragma once

#include "Asset/Asset.h"

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Engine
{
	class ShaderBase
	{
	public:
		ShaderBase() = default;
		virtual ~ShaderBase() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string& GetName() const = 0;

		// Methods to upload uniforms
		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* value, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		int GetUniformLocation(const std::string& name);

	private:
		virtual uint32_t GetRendererID() const = 0;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};

	class ShaderCompilationError : public std::runtime_error
	{
	public:
		ShaderCompilationError(const std::string& message)
			: std::runtime_error(message) {}

	};

	class Shader : public Asset, public ShaderBase
	{
	public:
		ASSET_TYPE(AssetType::Shader)

		Shader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader() override;

		void Bind() const override;
		void Unbind() const override;

		const std::string& GetName() const override { return m_Name; }

	private:
		uint32_t GetRendererID() const override { return m_RendererID; }

		uint32_t m_RendererID;
		std::string m_Name;
	};

	class ComputeShader : public Asset, public ShaderBase
	{
	public:
		ASSET_TYPE(AssetType::ComputeShader)

		ComputeShader(std::string name, const std::string& computeSrc);
		~ComputeShader() override;

		void Bind() const override;
		void Unbind() const override;

		const std::string& GetName() const override { return m_Name; }

	private:
		uint32_t GetRendererID() const override { return m_RendererID; }
		uint32_t m_RendererID;
		std::string m_Name;
	};
}