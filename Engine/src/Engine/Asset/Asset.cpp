#include "PCH.h"
#include "Asset.h"

namespace Engine
{
	std::string_view AssetTypeToString(enum AssetType type)
	{
		switch ( type )
		{
		case AssetType::None: return "AssetType::None";
		case AssetType::String: return "AssetType::String";
		case AssetType::Shader: return "AssetType::Shader";
		case AssetType::ComputeShader: return "AssetType::ComputeShader";
		case AssetType::Texture2D: return "AssetType::Texture2D";
		case AssetType::Custom: return "AssetType::Custom";
		}

		return "AssetType::Invalid";
	}

	AssetType StringToAssetType(std::string_view type)
	{
		if ( type == "AssetType::None" ) return AssetType::None;
		if ( type == "AssetType::String" ) return AssetType::String;
		if ( type == "AssetType::Shader" ) return AssetType::Shader;
		if ( type == "AssetType::ComputeShader" ) return AssetType::ComputeShader;
		if ( type == "AssetType::Texture2D" ) return AssetType::Texture2D;
		if ( type == "AssetType::Custom" ) return AssetType::Custom;

		return AssetType::None;
	}
}
