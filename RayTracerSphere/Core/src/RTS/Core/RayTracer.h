#pragma once

#include "RTS/Core/RenderTargetType.h"
#include "Engine.h"

#include <cstdint>

namespace RTS
{
	// Base class for the Ray Tracer to inherit from
	class RayTracer
	{
	public:
		virtual ~RayTracer() = default;

		virtual void Update() = 0;
		virtual void BindTexture(uint32_t slot, RenderTargetType type = RenderTargetType::None) = 0;
		virtual void OnImGUIRender() = 0;
		virtual void OnResize(uint32_t width, uint32_t height) = 0;

		virtual Engine::Buffer GetImageBuffer(RenderTargetType type = RenderTargetType::None) = 0; // Render Buffer but converted to 8-bit image
	};
}