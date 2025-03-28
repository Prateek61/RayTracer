#include "CPURaytracer.h"
#include "Scene.h"
#include "Sphere.h"
#include "RayTracer/Utils.h"
#include "RayTracer/Material.h"

namespace RT
{
	Scene::Scene()
		: m_World()
	{
		auto material_ground = std::make_shared<Lambertian>(glm::vec3{ 0.8f, 0.8f, 0.0f });
		auto material_center = std::make_shared<Lambertian>(glm::vec3{ 0.1f, 0.2f, 0.5f });
		auto material_left = std::make_shared<Metal>(glm::vec3{ 0.8f, 0.8f, 0.8f });
		auto material_right = std::make_shared<Metal>(glm::vec3{ 0.8f, 0.6f, 0.2f });

		m_World.Add(
			std::make_shared<Sphere>(glm::vec3{ 0.0f, -100.5f, -1.0f }, 100.0f, material_ground)
		);
		m_World.Add(
			std::make_shared<Sphere>(glm::vec3{ 0.0f, 0.0f, -1.2f }, 0.5f, material_center)
		);
		m_World.Add(
			std::make_shared<Sphere>(glm::vec3{ -1.0f, 0.0f, -1.0f }, 0.5f, material_left)
		);
		m_World.Add(
			std::make_shared<Sphere>(glm::vec3{ 1.0f, 0.0f, -1.0f }, 0.5f, material_right)
		);
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

	glm::vec3 Scene::RayColor(const Ray& r, uint32_t depth)
	{
		if (depth <= 0)
		{
			return glm::vec3(0.0f);
		}

		HitRecord rec;
		if (m_World.Hit(r, Interval(0.001f, INF), rec))
		{
			Ray scattered;
			glm::vec3 attenuation;
			if (rec.Mat->Scatter(r, rec, attenuation, scattered))
				return attenuation * RayColor(scattered, depth - 1);
			return glm::vec3(0.0f);
		}

		glm::vec3 unit_direction = glm::normalize(r.direction());
		float a = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}