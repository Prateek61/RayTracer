#pragma once

#include <cstdint>

namespace Engine
{
	struct Buffer
	{
		uint8_t* Data;
		uint64_t Size;

		Buffer() = default;
		Buffer(uint64_t size);
		Buffer(const Buffer&) = default;
		static Buffer Copy(Buffer other);
		void Allocate(uint64_t size);
		void Release();

		template <typename T>
		T* As();
		operator bool() const;
	};

	struct ScopedBuffer
	{
		ScopedBuffer(Buffer buffer);
		ScopedBuffer(uint64_t size);
		~ScopedBuffer();

		uint8_t* Data();
		uint64_t Size();

		template <typename T>
		T* As();

		operator bool() const;

	private:
		Buffer m_Buffer;
	};
}

// Template definitions
namespace Engine
{
	template <typename T>
	T* Buffer::As()
	{
		return reinterpret_cast<T*>(Data);
	}

	template <typename T>
	T* ScopedBuffer::As()
	{
		return m_Buffer.As<T>();
	}
}