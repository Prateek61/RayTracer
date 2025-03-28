#include "Vector.h"

namespace RTS
{
	bool VectorUtils::NearZero(const Vec3& vec)
	{
		return (std::fabs(vec.r) < s_epsilon) && (std::fabs(vec.g) < s_epsilon) && (std::fabs(vec.b) < s_epsilon);
	}

	uint32_t VectorUtils::ConvertToPixel(const Color& color)
	{
		return ConvertToPixel(Color4(color, 1.0f));
	}

	uint32_t VectorUtils::ConvertToPixel(const Color4& color)
	{
		const uint32_t red = static_cast<uint32_t>(glm::clamp(color.r, 0.0f, 0.999f) * 256.0f);
		const uint32_t green = static_cast<uint32_t>(glm::clamp(color.g, 0.0f, 0.999f) * 256.0f);
		const uint32_t blue = static_cast<uint32_t>(glm::clamp(color.b, 0.0f, 0.999f) * 256.0f);
		const uint32_t alpha = static_cast<uint32_t>(glm::clamp(color.a, 0.0f, 0.999f) * 256.0f);
		const uint32_t pixel = (alpha << 24) | (blue << 16) | (green << 8) | red;
		return pixel;
	}

}