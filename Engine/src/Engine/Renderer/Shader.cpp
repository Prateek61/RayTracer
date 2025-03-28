#include "PCH.h"
#include "Shader.h"

#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	void ShaderBase::SetInt(const std::string& name, int value)
	{
		PROFILE_FUNCTION();

		glUniform1i(GetUniformLocation(name), value);
	}

	void ShaderBase::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		PROFILE_FUNCTION();

		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void ShaderBase::SetFloat(const std::string& name, float value)
	{
		PROFILE_FUNCTION();

		glUniform1f(GetUniformLocation(name), value);
	}

	void ShaderBase::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		PROFILE_FUNCTION();

		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void ShaderBase::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		PROFILE_FUNCTION();

		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void ShaderBase::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		PROFILE_FUNCTION();

		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void ShaderBase::SetMat4(const std::string& name, const glm::mat4& value)
	{
		PROFILE_FUNCTION();

		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value_ptr(value));
	}

	int ShaderBase::GetUniformLocation(const std::string& name)
	{
		PROFILE_FUNCTION();

		auto itr = m_UniformLocationCache.find(name);
		if ( itr == m_UniformLocationCache.end() )
		{
			int location = glGetUniformLocation(GetRendererID(), name.c_str());
			m_UniformLocationCache[name] = location;
			return location;
		}

		return itr->second;
	}

	Shader::Shader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(std::move(name))
	{
		PROFILE_FUNCTION();

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, nullptr);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		// Check for vertex shader compile errors
		int isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if ( isCompiled == GL_FALSE )
		{
			int maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			LOG_CORE_ERROR("{0}", infoLog.data());
			throw ShaderCompilationError("Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, nullptr);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		// Check for fragment shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		if ( isCompiled == GL_FALSE )
		{
			int maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			LOG_CORE_ERROR("{0}", infoLog.data());
			throw ShaderCompilationError("Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.

		m_RendererID = glCreateProgram();

		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		int isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if ( isLinked == GL_FALSE )
		{
			int maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			LOG_CORE_ERROR("{0}", infoLog.data());
			throw ShaderCompilationError("Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	Shader::~Shader()
	{
		PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(0);
	}

	ComputeShader::ComputeShader(std::string name, const std::string& computeSrc)
		: m_Name(std::move(name))
	{
		PROFILE_FUNCTION();

		// Create an empty compute shader handle
		GLuint compute_shader = glCreateShader(GL_COMPUTE_SHADER);

		// Send the compute shader source code to GL
		const GLchar* source = computeSrc.c_str();
		glShaderSource(compute_shader, 1, &source, nullptr);

		// Compile the compute shader
		glCompileShader(compute_shader);

		// Check for compute shader compile errors
		int is_compiled = 0;
		glGetShaderiv(compute_shader, GL_COMPILE_STATUS, &is_compiled);
		if ( is_compiled == GL_FALSE )
		{
			int max_length = 0;
			glGetShaderiv(compute_shader, GL_INFO_LOG_LENGTH, &max_length);

			// The max_length includes the NULL character
			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(compute_shader, max_length, &max_length, info_log.data());

			// We don't need the shader anymore.
			glDeleteShader(compute_shader);

			LOG_CORE_ERROR("{0}", info_log.data());
			throw ShaderCompilationError("Compute shader compilation failure!");
			return;
		}

		// Compute shader is successfully compiled.
		// Now time to link it into a program.
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, compute_shader);

		// Link da program
		glLinkProgram(m_RendererID);

		// Check for compute shader link errors
		int is_linked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &is_linked);
		if ( is_linked == GL_FALSE )
		{
			int max_length = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &max_length);

			// The max_length includes the NULL character
			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(m_RendererID, max_length, &max_length, info_log.data());

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(compute_shader);

			LOG_CORE_ERROR("{0}", info_log.data());
			throw ShaderCompilationError("Compute shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, compute_shader);
	}

	ComputeShader::~ComputeShader()
	{
		PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void ComputeShader::Bind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void ComputeShader::Unbind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(0);
	}
}
