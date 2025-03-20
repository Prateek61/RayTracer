#include "CPURaytracer.h"
#include "CPURaytracer.h"
#include "Scene.h"

namespace RT
{
	float Scene::HitSphere(const glm::vec3& center, float radius, const Ray& r)
	{
		glm::vec3 oc = center - r.origin();
		// a = direction.length^2
		float a = glm::dot(r.direction(), r.direction());
		float h = glm::dot(r.direction(), oc);
		float c = glm::dot(oc, oc) - radius * radius;
		float discriminant = h * h - a * c;

		if (discriminant < 0.0f)
		{
			return -1.0f;
		}


		return (h - std::sqrt(discriminant)) / a;
	}

	glm::vec3 Scene::RayColor(const Ray& r)
	{
		float t = HitSphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, r);
		if (t > 0.0)
		{
			glm::vec3 normal = glm::normalize(r.at(t) - glm::vec3(0.0f, 0.0f, -1.0f));
			return 0.5f * glm::vec3(normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f);
		}

		glm::vec3 unit_direction = glm::normalize(r.direction());
		float a = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}