#pragma once

#include "Asset/Asset.h"
#include <string_view>

namespace Engine
{
	// Base class for all asset metadata
	struct AssetMetadata
	{
	public:
		std::string Name;

	public:
		AssetMetadata() = default;
		AssetMetadata(std::string name);
		virtual ~AssetMetadata() = default;

		std::string_view GetAssetTypeStr() const;

		static AssetType GetStaticType() { return AssetType::None; }
		virtual AssetType GetType() const = 0;
		virtual void AddBasePath(const std::filesystem::path& basePath) = 0;
	};

	class AssetLoadingError : public std::exception
	{
	public:
		AssetLoadingError(const AssetType type, const std::string& message)
			: m_Type(type)
		{
			m_Message = "Failed to load Asset " + std::string(AssetTypeToString(type)) + ": " + message;
		}

		AssetType GetType() const { return m_Type; }

		const char* what() const noexcept override { return m_Message.c_str(); }
	private:
		AssetType m_Type;
		std::string m_Message;
	};
}