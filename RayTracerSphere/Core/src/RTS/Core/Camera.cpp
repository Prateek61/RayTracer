#include "Camera.h"
#include "Engine.h"

#include "imgui.h"

namespace RTS
{
	Camera::Camera(uint32_t width, uint32_t height)
	{
		SetSize(width, height);
	}

	bool Camera::ImGuiProperties()
	{
		PROFILE_FUNCTION();

		bool modified = false;

		// FOV
		modified |= ImGui::DragFloat("FOV", &m_FOV, 0.1f, 0.1f, 100.0f);

		ImGui::Spacing();

		// LookFrom, LookAt, Up
		modified |= ImGui::DragFloat3("LookFrom", &m_LookFrom.x, 0.1f);
		modified |= ImGui::DragFloat3("LookAt", &m_LookAt.x, 0.1f);
		modified |= ImGui::DragFloat3("Up", &m_Up.x, 0.1f);

		ImGui::Spacing();

		// Focus
		modified |= ImGui::DragFloat("Defocus Angle", &m_DefocusAngle, 0.1f, 0.0f, 10.0f);
		modified |= ImGui::DragFloat("Focus Distance", &m_FocusDistance, 0.1f, 0.0f, 100.0f);

		if (modified)
		{
			initialize();
		}

		return modified;
	}

	void Camera::SetSize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		initialize();
	}

	Ray Camera::GetRay(uint32_t i, uint32_t j) const
	{
		PROFILE_FUNCTION();

		Vector offset = SampleSquare();
		Point pixel_sample = m_Pixel00_Loc
							+ (m_PixelDeltaU * (static_cast<float>(i) + offset.x))
							+ (m_PixelDeltaV * (static_cast<float>(j) + offset.y));

		Point ray_origin = (m_DefocusAngle <= 0.0f) ? m_Center : DefocusDiskSample();
		Vector ray_direction = glm::normalize(pixel_sample - ray_origin);

		return Ray(ray_origin, ray_direction);
	}

	void Camera::initialize()
	{
		PROFILE_FUNCTION();

		m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

		m_Center = m_LookFrom;

		// Determine Viewport Direction
		float theta = glm::radians(m_FOV);
		float half_height = glm::tan(theta / 2.0f);
		float viewport_height = 2.0f * half_height * m_FocusDistance;
		float viewport_width = m_AspectRatio * viewport_height;

		// Calculate the U, V, W basis vectors
		m_W = glm::normalize(m_LookFrom - m_LookAt);
		m_U = glm::normalize(glm::cross(m_Up, m_W));
		m_V = glm::cross(m_W, m_U);

		// Calculate the vectors across the horizontal and down the vertical viewport edges
		Vector viewport_u = m_U * viewport_width;
		Vector viewport_v = m_V * viewport_height;

		// Calculate the pixel deltas
		m_PixelDeltaU = viewport_u / static_cast<float>(m_Width);
		m_PixelDeltaV = viewport_v / static_cast<float>(m_Height);

		// Calculate the pixel 0,0 location
		Point viewport_upper_left = m_Center - (m_FocusDistance * m_W) - (viewport_u / 2.0f) - (viewport_v / 2.0f);
		m_Pixel00_Loc = viewport_upper_left + (m_PixelDeltaU / 2.0f) + (m_PixelDeltaV / 2.0f);

		// Calculate the defocus disk vectors
		float defocus_radius = m_FocusDistance * glm::tan(glm::radians(m_DefocusAngle / 2));
		m_DefocusDiskU = m_U * defocus_radius;
		m_DefocusDiskV = m_V * defocus_radius;
	}

	Point Camera::DefocusDiskSample() const
	{
		// Returns a random point on the defocus disk
		Vector p = VectorUtils::RandomInUnitDisk();
		return m_Center + (m_DefocusDiskU * p.x) + (m_DefocusDiskV * p.y);
	}

	Vector Camera::SampleSquare() const
	{
		return { Engine::Random::Float() - 0.5f, Engine::Random::Float() - 0.5f, 0.0f };
	}
}