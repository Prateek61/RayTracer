#include "PCH.h"
#include "AssetMetadata.h"

namespace Engine
{
	AssetMetadata::AssetMetadata(std::string name)
		: Name(std::move(name))
	{
	}

	std::string_view AssetMetadata::GetAssetTypeStr() const
	{
		return AssetTypeToString(GetType());
	}
}