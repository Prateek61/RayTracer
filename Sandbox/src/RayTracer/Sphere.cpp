#include "Sphere.h"

#include "CPURaytracer.h"

namespace RT
{
	bool Sphere::Hit(const Ray& r, Interval rayT, HitRecord& rec) const
	{
		glm::vec3 oc = m_Center - r.Origin();
		float a = glm::dot(r.Direction(), r.Direction());
		float h = glm::dot(r.Direction(), oc);
		float c = glm::dot(oc, oc) - m_Radius * m_Radius;

		float discriminant = h * h - a * c;
		if (discriminant < 0.0f)
		{
			return false;
		}

		float sqrt_d = std::sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range
		float root = (h - sqrt_d) / a;
		if (!rayT.surrounds(root))
		{
			root = (h + sqrt_d) / a;
			if (!rayT.surrounds(root))
			{
				return false;
			}
		}

		rec.T = root;
		rec.P = r.At(rec.T);
		rec.Mat = m_Mat;
		glm::vec3 OutwardNormal = (rec.P - m_Center) / m_Radius;
		rec.SetFaceNormal(r, OutwardNormal);

		return true;
	}
}
