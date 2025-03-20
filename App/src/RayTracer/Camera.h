#pragma once

#include <glm/glm.hpp>

namespace RT
{
	class Camera
	{
	public:
		float AspectRatio;
		uint32_t Width, Height;
		float ViewportHeight, ViewportWidth;
		glm::vec3 CameraCenter;
		glm::vec3 ViewportU, ViewportV;
		glm::vec3 PixelDeltaU, PixelDeltaV;
		glm::vec3 UpperLeft;
		glm::vec3 Pixel00Loc;

		Camera(uint32_t width, uint32_t height, const glm::vec3& cameraPosition, float viewportHeight);

		void UpdateCamera();
	};
}