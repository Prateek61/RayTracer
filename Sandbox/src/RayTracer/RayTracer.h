#pragma once

#include <cstdint>

namespace RT
{
	// Base class for the CPU and GPU ray tracers to inherit from
	class RayTracer
	{
	public:
		RayTracer(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height)
		{
		}

		virtual ~RayTracer() = default;

		virtual void BindTexture(const uint32_t slot) const = 0;
		virtual void Render() = 0;

	protected:
		uint32_t m_Width, m_Height;
	};
}