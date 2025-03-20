#include "HittableList.h"

namespace RT
{
	bool HittableList::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
	{
		HitRecord tempRec;
		bool hitAnything = false;
		float closestSoFar = tMax;

		for (const auto& object : m_Objects)
		{
			if (object->Hit(r, tMin, closestSoFar, tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.T;
				rec = tempRec;
			}
		}

		return hitAnything;
	}
}


