#pragma once

#include "RayTracer/Base.h"

namespace RT
{
	class Material;

	class HitRecord
	{
	public:
		glm::vec3 P;
		glm::vec3 Normal;
		std::shared_ptr<Material> Mat;
		float T;
		bool FrontFace;

		void SetFaceNormal(const Ray& r, const glm::vec3& outwardNormal)
		{
			FrontFace = glm::dot(r.direction(), outwardNormal) < 0.0f;
			Normal = FrontFace ? outwardNormal : -outwardNormal;
		}
	};

	// Abstract class for all hittable objects
	class Hittable
	{
	public:
		virtual ~Hittable() = default;
		virtual bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
	};
}