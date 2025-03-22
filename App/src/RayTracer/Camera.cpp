#include "Camera.h"
#include "imgui.h"
#include "Engine.h"

namespace RT
{
	Camera::Camera(uint32_t width, uint32_t height, const glm::vec3& cameraPosition, float viewportHeight, float focal_length)
	{
		Width = width;
		Height = height;
		AspectRatio = static_cast<float>(Width) / static_cast<float>(Height);

		FocalLength = focal_length;
		ViewportHeight = viewportHeight;
		ViewportWidth = AspectRatio * ViewportHeight;

		CameraCenter = cameraPosition;

		CalculateViewport();
		CalculatePixelDelta();
		CalculateUpperLeft();
		CalculatePixel00Loc();
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

	void Camera::SetViewportWidth(float width)
	{
		ViewportWidth = width;
		ViewportHeight = ViewportWidth / AspectRatio;

		CalculateViewport();
		CalculatePixelDelta();
		CalculateUpperLeft();
		CalculatePixel00Loc();

		Changed = true;
	}

	void Camera::SetViewportHeight(float width)
	{
		ViewportHeight = width;
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

		CalculateUpperLeft();
		CalculatePixel00Loc();

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

		// Viewport Width and Height
		if (ImGui::SliderFloat("Viewport Width", &ViewportWidth, 0.1f, 10.0f))
		{
			SetViewportWidth(ViewportWidth);
		}
		if (ImGui::SliderFloat("Viewport Height", &ViewportHeight, 0.1f, 10.0f))
		{
			SetViewportHeight(ViewportHeight);
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