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
		Ray GetRay(uint32_t i, uint32_t j) const;
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