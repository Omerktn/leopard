#pragma once

#include <cstring>
#include <memory>
#include <type_traits>

namespace leo
{

class Buffer
{
public:
	explicit Buffer(size_t capacity)
		: capacity{capacity}
		, consumedSize{0}
		, size{0}
		, buffer{new char[capacity]}
	{}

	~Buffer()
	{
		delete[] buffer;
	}
	size_t getCapacity() const
	{
		return capacity;
	}

	size_t getWrittenSize() const
	{
		return size;
	}

	const char* getData() const
	{
		return std::launder(buffer);
	}

	size_t getReadableSize() const
	{
		return size - consumedSize;
	}

	template <typename T>
	bool write(const T& object)
	{
		static_assert(std::is_standard_layout_v<std::decay_t<T>>);

		static constexpr auto OBJECT_SIZE = sizeof(T);
		if (getWrittenSize() + OBJECT_SIZE > getCapacity())
		{
			return false;
		}

		std::memcpy(getPointerToWrite(), &object, sizeof(T));
		size += sizeof(T);
		return true;
	}

	template <typename T>
	const T& get()
	{
		static constexpr auto OBJECT_SIZE = sizeof(T);
		if (getReadableSize() < OBJECT_SIZE)
		{
			throw std::runtime_error("Trying to get " + std::to_string(OBJECT_SIZE) +
									 " bytes but only " + std::to_string(getReadableSize()) +
									 " left in the buffer.");
		}

		const auto* resultPtr = reinterpret_cast<const T*>(getPointerToRead());
		consume(OBJECT_SIZE);
		return *resultPtr;
	}

	template <typename T>
	const T* peek()
	{
		if (getReadableSize() < sizeof(T))
		{
			return nullptr;
		}
		return reinterpret_cast<const T*>(getPointerToRead());
	}

	void consume(size_t consumedBytes)
	{
		if (consumedSize + consumedBytes > size)
		{
			throw std::runtime_error(
				"Attempted to consume too much. ConsumedSize=" + std::to_string(consumedSize) +
				" ConsumedBytes=" + std::to_string(consumedBytes) +
				" > Size=" + std::to_string(size));
		}

		consumedSize += consumedBytes;
	}

	void reset()
	{
		size = 0;
		consumedSize = 0;
	}

private:
	char* getData()
	{
		return std::launder(buffer);
	}

	char* getPointerToWrite()
	{
		return getData() + size;
	}

	const char* getPointerToRead() const
	{
		return getData() + consumedSize;
	}

	size_t capacity;
	size_t consumedSize;
	size_t size;
	char* buffer;
};
} // namespace leo