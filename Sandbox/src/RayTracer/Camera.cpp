#include "Camera.h"
#include "imgui.h"
#include "Engine.h"

namespace RT
{
	Camera::Camera(uint32_t width, uint32_t height, const glm::vec3& cameraPosition, float verticalFOV, float focal_length)
	{
		Width = width;
		Height = height;
		AspectRatio = static_cast<float>(Width) / static_cast<float>(Height);

		VerticalFOV = verticalFOV;
		FocalLength = focal_length;

		CameraCenter = cameraPosition;

		float theta = glm::radians(verticalFOV);
		float h = glm::tan(theta / 2.0f);

		ViewportHeight = 2.0f * h * FocalLength;
		ViewportWidth = AspectRatio * ViewportHeight;

		CalculateViewport();
		CalculatePixelDelta();
		CalculateUpperLeft();
		CalculatePixel00Loc();
	}

	void Camera::SetFOV(float fov)
	{
		float theta = glm::radians(fov);
		float h = glm::tan(theta / 2.0f);

		ViewportHeight = 2.0f * h * FocalLength;
		ViewportWidth = AspectRatio * ViewportHeight;

		CalculateViewport();
		CalculatePixelDelta();
		CalculateUpperLeft();
		CalculatePixel00Loc();

		Changed = true;
	}

	void Camera::SetSize(uint32_t width, uint32_t height)
	{
		Width = width;
		Height = height;
		AspectRatio = static_cast<float>(Width) / static_cast<float>(Height);
		ViewportWidth = AspectRatio * ViewportHeight;

		CalculateViewport();
		CalculatePixelDelta();
		CalculateUpperLeft();
		CalculatePixel00Loc();

		Changed = true;
	}

	void Camera::SetFocalLength(float focal_length)
	{
		FocalLength = focal_length;

		SetFOV(VerticalFOV);

		Changed = true;
	}

	void Camera::SetCameraCenter(const glm::vec3& center)
	{
		CameraCenter = center;

		CalculateUpperLeft();
		CalculatePixel00Loc();

		Changed = true;
	}

	void Camera::OnImGuiRender()
	{
		// Focal Length
		if (ImGui::SliderFloat("Focal Length", &FocalLength, 0.1f, 10.0f))
		{
			SetFocalLength(FocalLength);
		}

		// FOV
		if (ImGui::SliderFloat("Vertical FOV", &VerticalFOV, 1.0f, 180.0f))
		{
			SetFOV(VerticalFOV);
		}

		// Camera Center
		if (ImGui::SliderFloat("Camera Center X", &CameraCenter.x, -10.0f, 10.0f))
		{
			SetCameraCenter(CameraCenter);
		}
		if (ImGui::SliderFloat("Camera Center Y", &CameraCenter.y, -10.0f, 10.0f))
		{
			SetCameraCenter(CameraCenter);
		}
		if (ImGui::SliderFloat("Camera Center Z", &CameraCenter.z, 0.0f, 10.0f))
		{
			SetCameraCenter(CameraCenter);
		}
	}

	Ray Camera::GetRay(uint32_t x, uint32_t y) const
	{
		auto offset = SampleSquare();
		glm::vec3 pixel_sample = Pixel00Loc
			+ ((static_cast<float>(x) + offset.x) * PixelDeltaU)
			+ ((static_cast<float>(y) + offset.y) * PixelDeltaV);

		glm::vec3 direction = pixel_sample - CameraCenter;
		return Ray{ CameraCenter, direction };
	}

	glm::vec3 Camera::SampleSquare() const
	{
		return glm::vec3{ Engine::Random::Float() - 0.5, Engine::Random::Float() - 0.5, 0.0f };
	}

	void Camera::CalculateViewport()
	{
		ViewportU = glm::vec3{ ViewportWidth, 0, 0 };
		ViewportV = glm::vec3{ 0, ViewportHeight, 0 };
	}

	void Camera::CalculatePixelDelta()
	{
		PixelDeltaU = ViewportU / static_cast<float>(Width);
		PixelDeltaV = ViewportV / static_cast<float>(Height);
	}

	void Camera::CalculateUpperLeft()
	{
		UpperLeft = CameraCenter - glm::vec3{ 0, 0, FocalLength } - (ViewportU / 2.0f) - (ViewportV / 2.0f);
	}

	void Camera::CalculatePixel00Loc()
	{
		Pixel00Loc = UpperLeft + 0.5f * (PixelDeltaU + PixelDeltaV);
	}
}