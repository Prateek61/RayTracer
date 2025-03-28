#include "PCH.h"
#include "Buffer.h"

namespace Engine
{
	Buffer::Buffer(uint64_t size)
		: Data(nullptr), Size(0)
	{
		Allocate(size);
	}

	Buffer Buffer::Copy(Buffer other)
	{
		PROFILE_FUNCTION();

		Buffer result(other.Size);
		memcpy(result.Data, other.Data, other.Size);
		return result;
	}

	void Buffer::Allocate(uint64_t size)
	{
		PROFILE_FUNCTION();
		Release();

		Data = new uint8_t[size];
		Size = size;
	}

	void Buffer::Release()
	{
		PROFILE_FUNCTION();

		delete[] Data;
		Data = nullptr;
		Size = 0;
	}

	Buffer::operator bool() const
	{
		return static_cast<bool>(Data);
	}

	ScopedBuffer::ScopedBuffer(Buffer buffer)
		: m_Buffer(buffer)
	{
	}

	ScopedBuffer::ScopedBuffer(uint64_t size)
		: m_Buffer(size)
	{
	}

	ScopedBuffer::~ScopedBuffer()
	{
		m_Buffer.Release();
	}

	uint8_t* ScopedBuffer::Data()
	{
		return m_Buffer.Data;
	}

	uint64_t ScopedBuffer::Size()
	{
		return m_Buffer.Size;
	}

	ScopedBuffer::operator bool() const
	{
		return m_Buffer;
	}

}