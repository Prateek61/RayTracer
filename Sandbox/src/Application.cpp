#include "Engine.h"
#include "MainLayer.h"
#include "Core/EntryPoint.h"

class App : public Engine::Application
{
public:
	App(const Engine::ApplicationProps& props)
		: Engine::Application(props)
	{
		PushLayer(new MainLayer(props.WindowProps.Width, props.WindowProps.Height, props.AssetsDirectory));
	}
};

Engine::Application* Engine::CreateApplication(Engine::ApplicationCommandLineArgs args)
{
	Engine::WindowProps window_props{
		"Ray Tracing",
		1280,
		720,
		true
	};
	Engine::ApplicationProps application_props{
		"Ray Tracing",
		"../Assets/",
		args,
		window_props
	};

	return new App(application_props);
}