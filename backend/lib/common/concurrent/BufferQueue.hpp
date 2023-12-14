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
	void putAll(T&&... objects)
	{
		std::lock_guard<std::mutex> lock(mutex);
		putAllImpl(objects...);
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
	void putAllImpl() {}

	template <typename T, typename... OtherTypes>
	void putAllImpl(T&& object, OtherTypes&&... rest)
	{
		getWritableBuffer().write<T>(std::forward<T>(object));
		putAllImpl(rest...);
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