#include "PCH.h"
#include "Application.h"

#include "imgui.h"

#include "Utils/TimeStep.h"
#include "Utils/Time.h"
#include "Renderer/RendererCommand.h"

namespace Engine
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationProps& props)
	{
		Init(props);
	}

	Application::~Application() = default;

	void Application::Run()
	{
		PROFILE_FUNCTION();
		m_LastFrameTime = Time::GetTime();

		while ( m_Running )
		{
			PROFILE_SCOPE("RunLoop");

			const float time = Time::GetTime();
			const TimeStep time_step = time - m_LastFrameTime;
			m_LastFrameTime = time;

			{
				PROFILE_SCOPE("LayerStack OnUpdate");

				for ( Layer* layer : m_LayerStack )
				{
					layer->OnUpdate(time_step);
				}
			}
	
			{
				PROFILE_SCOPE("LayerStack OnImGuiRender");
				
				m_ImGuiLayer->Begin();
				for ( Layer* layer : m_LayerStack )
				{
				    layer->OnImGuiRender(time_step);
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(this->OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(this->OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Init(const ApplicationProps& props)
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Props = props;

		// Window
		m_Window = std::make_unique<Window>(props.WindowProps);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		// Renderer
		RendererCommand::Init();

		// ImGui Layer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		PROFILE_FUNCTION();

		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}
}
