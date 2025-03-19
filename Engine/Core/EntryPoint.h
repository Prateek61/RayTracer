#pragma once

#include "Commons/Base.h"
#include "Core/Application.h"

namespace Engine
{
	extern Application* CreateApplication(ApplicationCommandLineArgs args);
}

int main(int argc, char** argv)
{
	Engine::Log::Init();

	PROFILE_BEGIN_SESSION("Startup", "Startup.profile.json");
	Engine::ApplicationCommandLineArgs args = { argc, argv };
	auto app = Engine::CreateApplication(args);
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Runtime", "Runtime.profile.json");
	app->Run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Shutdown", "Shutdown.profile.json");
	{
		PROFILE_SCOPE("main Shutdown");
		delete app;
	}
	PROFILE_END_SESSION();
}