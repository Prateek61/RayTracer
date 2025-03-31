#pragma once

#include "RTS/Common/Base.h"

namespace RTS
{
	class Camera
	{
	public:
		Camera(uint32_t width, uint32_t height);

		bool ImGuiProperties();
		void SetSize(uint32_t width, uint32_t height);
		inline Ray GetRay(uint32_t i, uint32_t j) const;
	private:
		// Main Attributes
		uint32_t m_Width, m_Height;
		float m_FOV = 90.0f;
		Point m_LookFrom{ 0.0f, 0.0f, 0.0f };
		Point m_LookAt{ 0.0f, 0.0f, -1.0f };
		Vector m_Up{ 0.0f, 1.0f, 0.0f };

		float m_DefocusAngle = 0.0f; // Variation angle of rays through each pixel
		float m_FocusDistance = 10.0f; // Distance from camera look-from point to plane of perfect focus

		// Derived Attributes
		float m_AspectRatio = 0.0f;
		Point m_Center{};
		Point m_Pixel00_Loc{};
		Vector m_PixelDeltaU{}, m_PixelDeltaV{};
		Vector m_U{}, m_V{}, m_W{};
		Vector m_DefocusDiskU{}, m_DefocusDiskV{};

	private:
		void initialize();

		Point DefocusDiskSample() const;
		Vector SampleSquare() const;
	};
}

// Inline Definitions
namespace RTS
{
	inline Ray Camera::GetRay(uint32_t i, uint32_t j) const
	{
		Vector offset = SampleSquare();
		Point pixel_sample = m_Pixel00_Loc
			+ (m_PixelDeltaU * (static_cast<float>(i) + offset.x))
			+ (m_PixelDeltaV * (static_cast<float>(j) + offset.y));

		Point ray_origin = (m_DefocusAngle <= 0.0f) ? m_Center : DefocusDiskSample();
		Vector ray_direction = glm::normalize(pixel_sample - ray_origin);

		return Ray(ray_origin, ray_direction);
	}

	inline Vector Camera::SampleSquare() const
	{
		return { Engine::Random::Float() - 0.5f, Engine::Random::Float() - 0.5f, 0.0f };
	}

	inline Point Camera::DefocusDiskSample() const
	{
		// Returns a random point on the defocus disk
		Vector p = VectorUtils::RandomInUnitDisk();
		return m_Center + (m_DefocusDiskU * p.x) + (m_DefocusDiskV * p.y);
	}
}