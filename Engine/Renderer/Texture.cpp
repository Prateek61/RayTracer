#include "PCH.h"
#include "Texture.h"

#include "glad/glad.h"

namespace Engine
{
	namespace OpenGLUtils
	{
		static GLenum ImageFormatToGLDataFormat(const ImageFormat format)
		{
			switch(format)
			{
			case ImageFormat::RB: return GL_RED;
			case ImageFormat::RGB8: return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			case ImageFormat::RGBA32F: return GL_RGBA;
			case ImageFormat::None: break;
			}

			CORE_ASSERT(false, "UnSupported image format")
			return 0;
		}

		static GLenum ImageFormatToGLInternalFormat(const ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RB: return GL_RED;
			case ImageFormat::RGB8: return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			case ImageFormat::RGBA32F: return GL_RGBA32F;
			case ImageFormat::None: break;
			}

			CORE_ASSERT(false, "UnSupported image format")
			return 0;
		}
	}


	Texture2D::Texture2D(const TextureSpecification& spec, Buffer data)
		: m_Specification(spec), m_Width(spec.Width), m_Height(spec.Height)
	{
		PROFILE_FUNCTION();

		m_InternalFormat = OpenGLUtils::ImageFormatToGLInternalFormat(spec.Format);
		m_DataFormat = OpenGLUtils::ImageFormatToGLDataFormat(spec.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, spec.GenerateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (data) SetData(data);
	}

	Texture2D::~Texture2D()
	{
		PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::SetData(Buffer data)
	{
		PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;

		CORE_ASSERT(data.Size == static_cast<uint64_t>(m_Width * m_Height * bpp), "Data must be entire texture!")
		glTextureSubImage2D(m_RendererID, 0, 0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height), m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

	Buffer Texture2D::GetData() const
	{
		PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		Buffer data(static_cast<uint64_t>(m_Width * m_Height * bpp));
		glGetTextureImage(m_RendererID, 0, m_DataFormat, GL_UNSIGNED_BYTE, static_cast<GLsizei>(data.Size), data.Data);
		return data;
	}
}

