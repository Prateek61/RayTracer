#include "Scene.h"

namespace RT
{
	bool Scene::HitSphere(const glm::vec3& center, float radius, const Ray& r)
	{
		glm::vec3 oc = r.origin() - center;
		float a = glm::dot(r.direction(), r.direction());
		float b = 2.0f * glm::dot(oc, r.direction());
		float c = glm::dot(oc, oc) - radius * radius;
		float discriminant = b * b - 4 * a * c;
		return discriminant > 0;
	}

	glm::vec3 Scene::RayColor(const Ray& r)
	{
		if (HitSphere(glm::vec3(0, 0, -1), 0.5, r))
		{
			return { 1.0f, 0.0f, 0.0f };
		}

		glm::vec3 unit_direction = glm::normalize(r.direction());
		float a = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}