#include "Engine.h"
#include "ExampleLayer.h"
#include "Core/EntryPoint.h"

class App : public Engine::Application
{
public:
	App(const Engine::ApplicationProps& props)
		: Engine::Application(props)
	{
		PushLayer(new ExampleLayer(props.WindowProps.Width, props.WindowProps.Height));
	}
};

Engine::Application* Engine::CreateApplication(Engine::ApplicationCommandLineArgs args)
{
	Engine::WindowProps window_props{
		"Example Application!!",
		1280,
		720,
		true
	};
	Engine::ApplicationProps application_props{
		"Example Application",
		"../Assets/",
		args,
		window_props
	};

	return new App(application_props);
}