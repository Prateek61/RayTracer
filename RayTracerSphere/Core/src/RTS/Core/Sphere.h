#pragma once

#include "RTS/Common/Base.h"

#include <string>
#include <cstdint>

namespace RTS
{
	struct Sphere
	{
		Point Center;
		float Radius;
		uint32_t MaterialID;
		std::string Name = "SampleSphere";
	};
}