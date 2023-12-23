#pragma once

#include <common/Buffer.hpp>

#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

namespace leo::concurrent
{

class BufferQueue
{
public:
	explicit BufferQueue(size_t capacity)
		: firstBuffer(capacity)
		, secondBuffer(capacity)
		, mutex{}
		, isFirstBufferWritable{true}
	{}

	template <typename... T>
	bool putAll(T&&... objects)
	{
		std::lock_guard<std::mutex> lock(mutex);
		return putAllImpl(objects...);
	}

	bool putFrom(const Buffer& anotherBuffer)
	{
		std::lock_guard<std::mutex> lock(mutex);
		return getWritableBuffer().writeFrom(anotherBuffer);
	}

	Buffer& takeBuffer()
	{
		std::lock_guard<std::mutex> lock(mutex);

		auto& writtenBuffer = getWritableBuffer();

		swapWritableBuffer();
		getWritableBuffer().reset(); // New one should be clean

		return writtenBuffer;
	}

private:
	bool putAllImpl()
	{
		return true;
	}

	template <typename T, typename... OtherTypes>
	bool putAllImpl(T&& object, OtherTypes&&... rest)
	{
		const auto currenWritten = getWritableBuffer().write<T>(std::forward<T>(object));
		const auto restWritten = putAllImpl(std::forward<T>(rest)...);
		return currenWritten && restWritten;
	}

	Buffer& getWritableBuffer() noexcept
	{
		return isFirstBufferWritable ? firstBuffer : secondBuffer;
	}

	Buffer& getReadableBuffer() noexcept
	{
		return isFirstBufferWritable ? secondBuffer : firstBuffer;
	}

	void swapWritableBuffer() noexcept
	{
		isFirstBufferWritable = !isFirstBufferWritable;
	}

private:
	Buffer firstBuffer;
	Buffer secondBuffer;
	std::mutex mutex;
	bool isFirstBufferWritable;
};

} // namespace leo::concurrent