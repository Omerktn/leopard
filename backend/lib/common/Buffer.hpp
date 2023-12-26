#pragma once

#include <cstring>
#include <memory>
#include <type_traits>
#include <vector>

namespace leo
{

class Buffer
{
public:
	explicit Buffer(size_t defaultSize)
		: defaultSize{defaultSize}
		, consumedSize{0}
		, writtenSize{0}
		, buffer(defaultSize, '\0')
	{}

	~Buffer() {}

	size_t getDefaultSize() const
	{
		return defaultSize;
	}

	size_t getWrittenSize() const
	{
		return writtenSize;
	}

	const uint8_t* getData() const
	{
		return buffer.data();
	}

	size_t getReadableSize() const
	{
		return getWrittenSize() - consumedSize;
	}

	template <typename T>
	bool write(const T& object)
	{
		static_assert(std::is_standard_layout_v<std::decay_t<T>>);

		static constexpr auto OBJECT_SIZE = sizeof(T);
		const auto newSize = getWrittenSize() + OBJECT_SIZE;

		buffer.resize(newSize, '\0');
		std::memcpy(getPointerToWrite(), &object, sizeof(T));

		writtenSize = newSize;
		return true;
	}

	bool writeFrom(const Buffer& other)
	{
		const auto otherSize = other.getReadableSize();
		const auto newSize = getWrittenSize() + otherSize;

		buffer.resize(newSize, '\0');
		std::memcpy(getPointerToWrite(), other.getPointerToRead(), otherSize);

		writtenSize = newSize;
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
		if (consumedSize + consumedBytes > getWrittenSize())
		{
			throw std::runtime_error(
				"Attempted to consume too much. ConsumedSize=" + std::to_string(consumedSize) +
				" ConsumedBytes=" + std::to_string(consumedBytes) +
				" > WrittenSize=" + std::to_string(getWrittenSize()));
		}

		consumedSize += consumedBytes;
	}

	void reset()
	{
		buffer.clear();
		writtenSize = 0;
		consumedSize = 0;
	}

private:
	uint8_t* getData()
	{
		return buffer.data();
	}

	uint8_t* getPointerToWrite()
	{
		return getData() + writtenSize;
	}

	const uint8_t* getPointerToRead() const
	{
		return getData() + consumedSize;
	}

	const size_t defaultSize;
	size_t consumedSize;
	size_t writtenSize;
	std::vector<uint8_t> buffer;
};
} // namespace leo