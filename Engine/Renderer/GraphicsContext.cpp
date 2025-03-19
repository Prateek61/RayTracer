#include "PCH.h"

#include "GraphicsContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Engine
{
	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void GraphicsContext::Init()
	{
		PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		CORE_ASSERT(status, "Failed to initialize Glad!")

		std::string version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		LOG_CORE_INFO("OpenGL Version: {0}", version);
		std::string vendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		LOG_CORE_INFO("OpenGL Vendor: {0}", vendor);
		std::string renderer(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		LOG_CORE_INFO("OpenGL Renderer: {0}", renderer);
	}

	void GraphicsContext::SwapBuffers()
	{
		PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}