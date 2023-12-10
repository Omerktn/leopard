#pragma once

#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

namespace leo::concurrent
{

template <typename Element>
class Queue
{
	static_assert(std::is_nothrow_move_constructible_v<Element>);
	static_assert(std::is_nothrow_move_assignable_v<Element>);

	using ElementContainer = std::vector<Element>;
	using Iterator = typename ElementContainer::iterator;
	using ConstIterator = typename ElementContainer::const_iterator;

	using BeginEndPair = std::pair<ConstIterator, ConstIterator>;

public:
	explicit Queue(size_t initialSize)
		: firstBuffer()
		, secondBuffer()
		, mutex{}
		, isFirstBufferWritable{true}
	{
		firstBuffer.reserve(initialSize);
		secondBuffer.reserve(initialSize);
	}

	void putOne(Element&& element)
	{
		std::lock_guard<std::mutex> lock(mutex);
		getWritableBuffer().push_back(std::forward<Element>(element));
	}

	BeginEndPair takeAll()
	{
		std::lock_guard<std::mutex> lock(mutex);

		auto& writtenBuffer = getWritableBuffer();

		swapWritableBuffer();
		getWritableBuffer().clear(); // New one should be clean

		return {writtenBuffer.cbegin(), writtenBuffer.cend()};
	}

private:
	ElementContainer& getWritableBuffer() noexcept
	{
		return isFirstBufferWritable ? firstBuffer : secondBuffer;
	}

	ElementContainer& getReadableBuffer() noexcept
	{
		return isFirstBufferWritable ? secondBuffer : firstBuffer;
	}

	void swapWritableBuffer() noexcept
	{
		isFirstBufferWritable = !isFirstBufferWritable;
	}

private:
	ElementContainer firstBuffer;
	ElementContainer secondBuffer;
	std::mutex mutex;
	bool isFirstBufferWritable;
};

} // namespace leo::concurrent