#include "PCH.h"
#include "RendererCommand.h"

#include "glad/glad.h"

namespace Engine
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch ( severity )
		{
		case GL_DEBUG_SEVERITY_HIGH: LOG_CORE_FATAL(message);
			return;
		case GL_DEBUG_SEVERITY_MEDIUM: LOG_CORE_ERROR(message);
			return;
		case GL_DEBUG_SEVERITY_LOW: LOG_CORE_WARN(message);
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(message);
			return;
		default: CORE_ASSERT(false, "Unknown severity level!");
		}
	}

	void RendererCommand::Init()
	{
		PROFILE_FUNCTION();

		DEBUG_ONLY
		(
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		)

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RendererCommand::SetClearColor(const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		PROFILE_FUNCTION();

		glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	void RendererCommand::Clear()
	{
		PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		PROFILE_FUNCTION();

		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RendererCommand::DispatchCompute(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
	{
		PROFILE_FUNCTION();

		glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
	}

	void RendererCommand::MemoryBarrierImageAccess()
	{
		PROFILE_FUNCTION();

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
}
