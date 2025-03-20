#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

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
		float FocalLength;

		Camera(uint32_t width, uint32_t height, const glm::vec3& cameraPosition, float viewportHeight, float focal_length = 1.0);

		void SetSize(uint32_t width, uint32_t height);
		void SetViewportWidth(float width);
		void SetViewportHeight(float width);
		void SetFocalLength(float focal_length);
		void SetCameraCenter(const glm::vec3& center);

		void OnImGuiRender();

		Ray GetRay(uint32_t x, uint32_t y) const;

	private:
		void CalculateViewport();
		void CalculatePixelDelta();
		void CalculateUpperLeft();
		void CalculatePixel00Loc();
	};
}