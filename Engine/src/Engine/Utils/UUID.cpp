#include "PCH.h"
#include "UUID.h"

#include "Engine/Utils/Random.h"

namespace Engine
{
	UUID::UUID()
		: m_UUID(Random::ULong())
	{
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}
}