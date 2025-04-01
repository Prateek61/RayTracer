 #include "SceneInteraction.h"

#include "RTS/Common/Base.h"
#include "RTS/Common/Base.h"
#include "RTS/Common/Base.h"
#include "RTS/Common/Base.h"

namespace RTS
{
	float SceneInteraction::IntersectSphere(const Sphere& sphere, const Ray& ray, Interval rayInterval)
	{
		Vector oc = sphere.Center - ray.Origin();

		float a = dot(ray.Direction(), ray.Direction());
		float h = dot(ray.Direction(), oc);
		float c = dot(oc, oc) - sphere.Radius * sphere.Radius;

		float discriminant = h * h - a * c;
		if (discriminant < 0) return -1.0f;

		float sqrt_d = glm::sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range.
		float root = (h - sqrt_d) / a;
		if (!rayInterval.Surrounds(root))
		{
			root = (h + sqrt_d) / a;
			if (!rayInterval.Surrounds(root))
			{
				return -1.0f;
			}
		}

		return root;
	}

	HitRecord SceneInteraction::TraceRay(const Ray& ray, Interval rayInterval)
	{
		int closest_sphere = -1;
		float hit_distance = RTS::INF;

		for (int i = 0; i < m_Scene->Spheres.size(); ++i)
		{
			float t = IntersectSphere(m_Scene->Spheres[i], ray, rayInterval);
			if (t > 0 && t < hit_distance)
			{
				hit_distance = t;
				closest_sphere = i;
			}
		}

		HitRecord hit_record;
		if (closest_sphere == -1)
		{
			hit_record.HitDistance = -1.0f;
			return hit_record;
		}

		hit_record.HitDistance = hit_distance;
		hit_record.ObjectIndex = closest_sphere;

		const Sphere& sphere = m_Scene->Spheres[closest_sphere];

		glm::vec3 oc = ray.Origin() - sphere.Center;
		hit_record.Position = ray.At(hit_distance);
		glm::vec3 outward_normal = (hit_record.Position - sphere.Center) / sphere.Radius;
		hit_record.SetFaceNormal(ray, outward_normal);
		hit_record.MaterialIndex = sphere.MaterialID;

		return hit_record;
	}

	Color Heatmap(float t)
	{
		t = glm::clamp(t, 0.0f, 1.0f);
		return (1.0f - t) * Color(1.0f, 0.0f, 0.0f)  // Red for low bounces
			+ t * Color(0.0f, 0.0f, 1.0f);         // Blue for high bounces
	}

	Color SceneInteraction::RayColor(const Ray& ray, Interval rayInterval, int bounces)
	{
		Ray scattered_ray = ray;

		Color attenuation = Color(1.0f);

		// Subsequent hits
		int i;
		for (i = 0; i < bounces; ++i)
		{
			HitRecord hit_record = TraceRay(scattered_ray, rayInterval);
			if (hit_record.HitDistance < 0.0f)
			{
				attenuation *= AmbientColor(scattered_ray);
				break;
			}

			Material& material = m_Scene->GetMaterial(hit_record.MaterialIndex);

			Ray new_ray = scattered_ray;
			if (!Scatter(new_ray, hit_record, material, scattered_ray))
			{
				attenuation *= Color{ 0.0f };
				break;
			}

			attenuation *= material.Albedo;

			rayInterval = { 0.01f, hit_record.HitDistance };
		}
		if (i == bounces)
		{
			return Color(0.0f);
		}

		//return Heatmap( static_cast<float>(i) / static_cast<float>(bounces));

		return attenuation;
	}

	Color SceneInteraction::BaseColor(const Ray& ray, Interval rayInterval)
	{
		auto hit_record = TraceRay(ray, rayInterval);
		if (hit_record.HitDistance < 0)
		{
			return AmbientColor(ray);
		}

		return m_Scene->GetMaterial(hit_record.MaterialIndex).Albedo;
	}

	float SceneInteraction::Depth(const Ray& ray, Interval rayInterval)
	{
		auto hit_record = TraceRay(ray, rayInterval);
		return hit_record.HitDistance;
	}

	bool SceneInteraction::Scatter(const Ray& ray, const HitRecord& hitRecord, const Material& material, Ray& scatteredRay)
	{
		Vector random = Engine::Random::InUnitSphere();

		// Diffuse
		//Vector diffuse = glm::normalize(hitRecord.Normal + material.Roughness * random);
		Vector diffuse = hitRecord.Normal + random;

		// Catch degenerate scatter direction
		if (VectorUtils::NearZero(diffuse))
		{
			diffuse = hitRecord.Normal;
		}

		/*if (material.Metallic <= 0.0f)
		{
			scatteredRay = { hitRecord.Position, diffuse };
			return true;
		}*/
		scatteredRay = { hitRecord.Position, diffuse };
		return true;


		// Metallic
		Vector reflected = glm::normalize( glm::reflect(glm::normalize(ray.Direction()), hitRecord.Normal));
		Vector metallic = glm::mix(diffuse, reflected, material.Metallic);

		scatteredRay = { hitRecord.Position, metallic };

		return glm::dot(scatteredRay.Direction(), hitRecord.Normal) > 0;
	}

	Color SceneInteraction::AmbientColor(const Ray& ray)
	{
		Vector uint_direction = glm::normalize(ray.Direction());
		float a = 0.5f * (uint_direction.y + 1.0f);
		return (1.0f - a) * Color(1.0f) + a * Color(0.5f, 0.7f, 1.0f);
	}
}
