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
		Metal(const glm::vec3& albedo)
			: m_Albedo(albedo)
		{}

		bool Scatter(const Ray& rIn, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
		{
			glm::vec3 reflected = glm::reflect(rIn.direction(), rec.Normal);
			scattered = Ray{ rec.P, reflected };
			attenuation = m_Albedo;
			return true;
		}

	private:
		glm::vec3 m_Albedo;
	};
}
