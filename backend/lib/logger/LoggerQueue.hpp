#pragma once

#include <common/concurrent/BufferQueue.hpp>

#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

namespace leo::logger
{

class LoggerQueue : private concurrent::BufferQueue
{
	static constexpr auto QUEUE_SIZE = 1024 * 512;

public:
	explicit LoggerQueue()
		: concurrent::BufferQueue{QUEUE_SIZE}
	{}

	template <typename T>
	void putLogEvent(T&& event)
	{}

private:
};

} // namespace leo::logger