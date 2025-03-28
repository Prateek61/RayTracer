#pragma once

#include "Engine/Commons/Base.h"
#include "Engine/Core/Props.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Events/ApplicationEvent.h"

// Forward declaration of main
int main(int argc, char** argv);

namespace Engine
{
	class Application
	{
	public:
		Application(const ApplicationProps& props);
		virtual ~Application();

		void OnEvent(Event& e);
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }

	private:
		ApplicationProps m_Props;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;
		float m_LastFrameTime = 0;

	private:
		void Init(const ApplicationProps& props);
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};
}
