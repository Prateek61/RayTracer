#pragma once

#include "Renderer/VertexArray.h"

#include "glm/glm.hpp"
#include <cstdint>

namespace Engine
{
	class RendererCommand
	{
	public:
		static void Init();
		static void SetClearColor(const glm::vec4& color);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void Clear();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DispatchCompute(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);
		static void MemoryBarrierImageAccess();
	};
}