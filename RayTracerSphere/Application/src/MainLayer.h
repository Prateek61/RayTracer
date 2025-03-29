#pragma once

#include "Engine.h"
#include "RayTracerSphere.h"

class MainLayer : public Engine::Layer
{
public:
	MainLayer()
		: Layer("MainLayer") {}
private:
	RTS::Scene m_Scene;
	//RTS::Camera m_Camera;
};