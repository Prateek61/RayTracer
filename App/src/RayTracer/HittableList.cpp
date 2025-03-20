#include "HittableList.h"

namespace RT
{
	bool HittableList::Hit(const Ray& r, Interval rayT, HitRecord& rec) const
	{
		HitRecord tempRec;
		bool hitAnything = false;
		float closestSoFar = rayT.Max;

		for (const auto& object : m_Objects)
		{
			if (object->Hit(r, Interval(rayT.Min, closestSoFar), tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.T;
				rec = tempRec;
			}
		}

		return hitAnything;
	}
}


