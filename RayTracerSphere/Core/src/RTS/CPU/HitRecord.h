#pragma once

#include "RTS/Common/Base.h"

namespace RTS
{
	struct HitRecord
	{
		float HitDistance = -1.0f;
		Point Position{ 0.0f };
		Vector Normal{ 0.0f };
		uint32_t MaterialIndex{ 0 };
		uint32_t ObjectIndex{ 0 };

		void SetFaceNormal(const Ray& ray, const Vector& outwardNormal)
		{
			//Normal = glm::faceforward(outwardNormal, outwardNormal, ray.Direction());
			bool frontFace = glm::dot(ray.Direction(), outwardNormal) < 0;
			Normal = frontFace ? outwardNormal : -outwardNormal;
		}
	};
}