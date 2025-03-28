#include "PCH.h"
#include "Time.h"

#include "GLFW/glfw3.h"

namespace Engine
{
	float Time::GetTime()
	{
		return static_cast<float>(glfwGetTime());
	}

}
