#pragma once

#include "Hittable.h"

namespace RT
{
	class Sphere : public Hittable
	{
	public:
		Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> mat)
			: m_Center(center), m_Radius(std::fmax(0.0f, radius)), m_Mat(mat)
		{
			// TODO: Initialize the material
		}

		bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const override;

		const glm::vec3& center() const { return m_Center; }
		float radius() const { return m_Radius; }
	private:
		glm::vec3 m_Center;
		float m_Radius;
		std::shared_ptr<Material> m_Mat;
	};
}