#include "Camera.h"

namespace RT
{
	Camera::Camera(uint32_t width, uint32_t height, const glm::vec3& cameraPosition, float viewportHeight)
		: Width(width), Height(height), ViewportHeight(viewportHeight), CameraCenter(cameraPosition)
	{
		UpdateCamera();
	}

	void Camera::UpdateCamera()
	{
		AspectRatio = static_cast<float>(Width) / static_cast<float>(Height);
		ViewportWidth = ViewportHeight * AspectRatio;
	}
}