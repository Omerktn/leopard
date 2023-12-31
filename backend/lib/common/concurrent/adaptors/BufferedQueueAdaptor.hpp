#pragma once

#include <common/Buffer.hpp>

namespace leo::concurrent
{

template <typename MainQueueType>
class BufferedQueueAdaptor
{

public:
	BufferedQueueAdaptor(MainQueueType& mainQueue, size_t initialSize)
		: mainQueue{mainQueue}
		, buffer{initialSize}
	{}

	template <typename... T>
	void putAll(T&&... objects)
	{
		putAllImpl(objects...);
	}

	void flush()
	{
		const auto size = buffer.getReadableSize();
		if (size == 0)
		{
			return;
		}

		const auto didWrite = mainQueue.putFrom(buffer);
		if (!didWrite)
		{
			unwrittenFlushAttempt += 1;
			unwrittenByteCount += size;
		}

		buffer.reset();
	}

	float getFullnessRatio() const
	{
		return static_cast<float>(buffer.getWrittenSize()) / buffer.getDefaultSize();
	}

	const Buffer& getBuffer() const
	{
		return buffer;
	}

private:
	void putAllImpl() {}

	template <typename T, typename... OtherTypes>
	void putAllImpl(T&& object, OtherTypes&&... rest)
	{
		buffer.write<T>(std::forward<T>(object));
		putAllImpl(rest...);
	}

private:
	MainQueueType& mainQueue;
	Buffer buffer;

	size_t unwrittenFlushAttempt = 0;
	size_t unwrittenByteCount = 0;
};

} // namespace leo::concurrent