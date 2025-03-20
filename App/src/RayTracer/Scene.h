#pragma once

#include "Ray.h"

namespace RT
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		float HitSphere(const glm::vec3& center, float radius, const Ray& r);
		glm::vec3 RayColor(const Ray& r);
	};
}