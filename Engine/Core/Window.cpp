#include "PCH.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

#include <cstdint>

namespace Engine
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::OnUpdate()
	{
		PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void Window::SetVSync(bool enabled)
	{
		PROFILE_FUNCTION();

		if ( enabled ) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_Props.VSync = enabled;
	}

	void Window::Init(const WindowProps& props)
	{
		PROFILE_FUNCTION();

		m_Props = props;

		LOG_CORE_INFO("Creating window {0} ({1}, {2})", m_Props.Title, m_Props.Width, m_Props.Height);

		if ( s_GLFWWindowCount == 0 )
		{
			PROFILE_SCOPE("Window::Init glfwInit");
			int success = glfwInit();
			CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			PROFILE_SCOPE("Window::Init glfwCreateWindow");

			DEBUG_ONLY
			(
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			)

			m_Window = glfwCreateWindow(static_cast<int>(m_Props.Width), static_cast<int>(m_Props.Height),
			                            m_Props.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = std::make_unique<GraphicsContext>(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, this);
		SetVSync(m_Props.VSync);

		SetGLFWCallbacks();
	}

	void Window::SetGLFWCallbacks()
	{
		PROFILE_FUNCTION();

		// Size Callback
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* glfw_window, int width, int height)
		{
			PROFILE_FUNCTION();

			auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
			window->m_Props.Width = width;
			window->m_Props.Height = height;

			WindowResizeEvent event(width, height);
			window->EventCallback(event);
		});

		// Close Callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* glfw_window)
		{
			PROFILE_FUNCTION();

			auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
			WindowCloseEvent event;
			window->EventCallback(event);
		});

		// Key callback
		glfwSetKeyCallback(m_Window, [](GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
		{
			PROFILE_FUNCTION();

			auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

			switch ( action )
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				window->EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				window->EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				window->EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* glfw_window, unsigned int keycode)
		{
			PROFILE_FUNCTION();

			auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

			KeyTypedEvent event(keycode);
			window->EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* glfw_window, int button, int action, int mods)
		{
			PROFILE_FUNCTION();

			auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

			switch ( action )
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				window->EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				window->EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* glfw_window, double xOffset, double yOffset)
		{
			PROFILE_FUNCTION();

			auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			window->EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* glfw_window, double xPos, double yPos)
		{
			PROFILE_FUNCTION();

			auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			window->EventCallback(event);
		});
	}

	void Window::Shutdown()
	{
		PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if ( s_GLFWWindowCount == 0 )
		{
			PROFILE_SCOPE("Window::Shutdown glfwTerminate");
			glfwTerminate();
		}
	}
}
