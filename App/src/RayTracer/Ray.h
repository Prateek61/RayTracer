#pragma once

#include <glm/glm.hpp>

namespace RT
{
	class Ray
	{
	public:
		Ray()
			: m_Orig(), m_Dir()
		{
		}

		Ray(const glm::vec3& origin, const glm::vec3& direction)
			: m_Orig(origin), m_Dir(direction)
		{
		}

		const glm::vec3& origin() const { return m_Orig; }
		const glm::vec3& direction() const { return m_Dir; }

		glm::vec3 at(float t) const
		{
			return m_Orig + t * m_Dir;
		}

	private:
		glm::vec3 m_Orig;
		glm::vec3 m_Dir;
	};
}