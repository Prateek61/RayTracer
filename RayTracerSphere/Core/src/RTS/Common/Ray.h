#pragma once

#include "RTS/Common/Vector.h"

namespace RTS
{
	class Ray
	{
	public:
		Ray() = default;

		Ray(const Point& origin, const Vector& direction)
			: m_Origin(origin), m_Direction(direction)
		{
		}

		const Point& Origin() const { return m_Origin; }
		const Vector& Direction() const { return m_Direction; }

		Point At(float t) const
		{
			return m_Origin + t * m_Direction;
		}

	private:
		Point m_Origin;
		Vector m_Direction;
	};
}
