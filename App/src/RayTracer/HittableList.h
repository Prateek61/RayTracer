#pragma once

#include "RayTracer/Base.h"
#include "RayTracer/Hittable.h"
#include <vector>

namespace RT
{
	class HittableList : public Hittable
	{
	public:
		std::vector<std::shared_ptr<Hittable>> m_Objects;

		HittableList() = default;
		HittableList(const std::shared_ptr<Hittable>& object) { Add(object); }
		void Clear() { m_Objects.clear(); }
		void Add(const std::shared_ptr<Hittable>& object) { m_Objects.emplace_back(object); }

		bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
	};
}
