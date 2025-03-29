#pragma once

#include "RTS/Common/Base.h"
#include "RTS/CPU/HitRecord.h"
#include "RTS/Core/Sphere.h"
#include "RTS/CPU/Interval.h"
#include "RTS/Core/Scene.h"

namespace RTS
{
	class SceneInteraction
	{
	public:
		SceneInteraction(Scene* scene) : m_Scene(scene) {}
		Scene* GetScene() const { return m_Scene; }

		static float IntersectSphere(const Sphere& sphere, const Ray& ray, Interval rayInterval);
		HitRecord TraceRay(const Ray& ray, Interval rayInterval);
		Color RayColor(const Ray& ray, Interval rayInterval, int bounces);
		Color BaseColor(const Ray& ray, Interval rayInterval);
		float Depth(const Ray& ray, Interval rayInterval);

	private:
		Scene* m_Scene;

	private:
		Color AmbientColor(const Ray& ray);
	};

	
}
