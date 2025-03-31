#include "PCH.h"
#include "Random.h"

namespace Engine
{
	std::mt19937 thread_local Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
}