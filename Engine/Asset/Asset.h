#pragma once

#include "Utils/UUID.h"
#include <string_view>
#include <cstdint>

namespace Engine
{
	using AssetHandle = UUID;
	const AssetHandle INVALID_ASSET_HANDLE = UUID(0);

	enum class AssetType : uint16_t
	{
		None = 0,
		String,
		Shader,
		ComputeShader,
		Texture2D,
		Custom
	};

	std::string_view AssetTypeToString(AssetType type);
	AssetType StringToAssetType(std::string_view type);

	// Base class for all assets
	class Asset
	{
	public:
		AssetHandle Handle = INVALID_ASSET_HANDLE;

	public:
		virtual ~Asset() = default;

		virtual AssetType GetType() const = 0;
	};
}

#define ASSET_TYPE(type) static AssetType GetStaticType() { return type; } \
	AssetType GetType() const override { return GetStaticType(); }
