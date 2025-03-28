#pragma once

#include "Base.h"
#include "Base.h"
#include "Base.h"
#include "Base.h"

#include "RayTracer/Base.h"
#include "RayTracer/HittableList.h"

namespace RT
{
	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		float HitSphere(const glm::vec3& center, float radius, const Ray& r);
		glm::vec3 RayColor(const Ray& r, uint32_t depth);

	private:
		HittableList m_World;
	};
}