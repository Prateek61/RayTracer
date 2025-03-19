#pragma once

#include "Asset/Asset.h"
#include "Utils/Buffer.h"

namespace Engine
{
	enum class ImageFormat
	{
		None = 0,
		RB,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMipmaps = true;
	};

	// Base texture class
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(Buffer data) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool IsLoaded() const = 0;
		virtual bool operator==(const Texture& other) const = 0;
		virtual Buffer GetData() const = 0;
	};

	class Texture2D final : public Asset, public Texture
	{
	public:
		ASSET_TYPE(AssetType::Texture2D);

		Texture2D(const TextureSpecification& spec, Buffer data = Buffer());
		~Texture2D() override;

		const TextureSpecification& GetSpecification() const override { return m_Specification; }

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetRendererID() const override { return m_RendererID; }

		void SetData(Buffer data) override;
		void Bind(uint32_t slot = 0) const override;
		Buffer GetData() const override;

		bool IsLoaded() const override { return m_Loaded; }
		bool operator==(const Texture& other) const override { return m_RendererID == dynamic_cast<const Texture2D&>(other).m_RendererID; }

	private:
		TextureSpecification m_Specification;

		bool m_Loaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		uint32_t m_InternalFormat, m_DataFormat;
	};
}