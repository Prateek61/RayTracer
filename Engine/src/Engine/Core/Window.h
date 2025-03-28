#pragma once

#include "Engine/Commons/Base.h"
#include "Engine/Core/Props.h"
#include "Engine/Events/Event.h"
#include "Engine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Engine
{
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		virtual ~Window();

		void OnUpdate();

		uint32_t GetWidth() const { return m_Props.Width; }
		uint32_t GetHeight() const { return m_Props.Height; }
		WindowProps GetProps() const { return m_Props; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVsync() const { return m_Props.VSync; }

		GLFWwindow* GetGLFWWindow() const { return m_Window; }
		void EventCallback(Event& e) { m_EventCallback(e); }

	private:
		void Init(const WindowProps& props);
		void SetGLFWCallbacks();
		void Shutdown();

	private:
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;
		WindowProps m_Props;
		EventCallbackFn m_EventCallback;
	};
}