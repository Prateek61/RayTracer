#pragma once

#include "Engine.h"
#include "RayTracer/Hittable.h"
#include "RayTracer/Utils.h"

namespace RT
{
	class Material
	{
	public:
		virtual ~Material() = default;

		virtual bool Scatter(const Ray& rIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const { return false; }
	};

	class Lambertian : public Material
	{
	public:
		Lambertian(const glm::vec3& albedo) : m_Albedo(albedo) {}

		bool Scatter(const Ray& rIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
		{
			auto scatter_direction = rec.Normal + Engine::Random::InUnitSphere();

			// Catch degenerate scatter direction
			if ( Utils::NearZero(scatter_direction) )
			{
				scatter_direction = rec.Normal;
			}

			scattered = Ray{ rec.P, scatter_direction };
			attenuation = m_Albedo;
			return true;
		}
	private:
		glm::vec3 m_Albedo;
	};

	class Metal : public Material
	{
	public:
		Metal(const glm::vec3& albedo, float fuzz = 0.0f)
			: m_Albedo(albedo), m_Fuzz(glm::clamp(fuzz, 0.0f, 1.0f))
		{}

		bool Scatter(const Ray& rIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
		{
			glm::vec3 reflected = glm::reflect(rIn.Direction(), rec.Normal);
			reflected = glm::normalize(reflected) + m_Fuzz * Engine::Random::InUnitSphere();
			scattered = Ray{ rec.P, reflected };
			attenuation = m_Albedo;
			return (glm::dot(scattered.Direction(), rec.Normal) > 0.0f);
		}

	private:
		glm::vec3 m_Albedo;
		float m_Fuzz;
	};

	class Dielectric : public Material
	{
	public:
		Dielectric(float refractionIndex) : m_RefractionIndex(refractionIndex) {}

		bool Scatter(const Ray& rIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
		{
			attenuation = glm::vec3(1.0f);
			float ri = rec.FrontFace ? (1.0f / m_RefractionIndex) : m_RefractionIndex;

			glm::vec3 unit_direction = glm::normalize(rIn.Direction());

			float cos_theta = std::fmin(glm::dot(-unit_direction, rec.Normal), 1.0f);
			float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

			bool cannot_refract = ri * sin_theta > 1.0f;
			glm::vec3 direction;

			if (cannot_refract || Reflectance(cos_theta, ri) > Engine::Random::Float())
			{
				direction = glm::reflect(unit_direction, rec.Normal);
			}
			else
			{
				direction = glm::refract(unit_direction, rec.Normal, ri);
			}

			scattered = Ray{ rec.P, direction };
			return true;
		}

	private:
		float m_RefractionIndex;

		static float Reflectance(float cosine, float refractionIndex)
		{
			// Use Schlick's approximation for reflectance
			float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
			r0 = r0 * r0;
			return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
		}
	};
}
