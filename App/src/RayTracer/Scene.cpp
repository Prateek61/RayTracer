#include "CPURaytracer.h"
#include "Scene.h"
#include "Sphere.h"

namespace RT
{
	Scene::Scene()
		: m_World()
	{
		m_World.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
		m_World.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));
		m_World.Add(std::make_shared<Sphere>(glm::vec3(4.0f, 2.0f, -3.0f), 1.0f));
	}

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
		HitRecord rec;
		if (m_World.Hit(r, 0.0f, INF, rec))
		{
			return 0.5f * (rec.Normal + glm::vec3(1.0f));
		}

		glm::vec3 unit_direction = glm::normalize(r.direction());
		float a = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}