#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "MainLayer.h"

class SphereTracer : public Engine::Application
{
public:
	SphereTracer(const Engine::ApplicationProps& props)
		: Engine::Application(props)
	{
		PushLayer(new MainLayer(props.WindowProps.Width, props.WindowProps.Height, props.AssetsDirectory));
	}
};

Engine::Application* Engine::CreateApplication(Engine::ApplicationCommandLineArgs args)
{
	Engine::WindowProps window_props{
		"Sphere Ray Tracing",
		1280,
		720,
		false
	};
	Engine::ApplicationProps application_props{
		"Sphere Ray Tracing",
		"../../Assets/",
		args,
		window_props
	};

	return new SphereTracer(application_props);
}