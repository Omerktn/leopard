#pragma once

#include <atomic>

namespace leo
{

class SpinLock
{
	std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
	explicit SpinLock(uint32_t maxLockAttempt)
		: maxLockAttempt{maxLockAttempt}
	{}

	bool lock()
	{
		uint32_t count = 0;
		bool success = false;

		while (success = locked.test_and_set(std::memory_order_acquire) && count < maxLockAttempt)
		{
			++count;
		}

		return success;
	}

	void lockForce()
	{
		while (locked.test_and_set(std::memory_order_acquire))
		{}
	}

	void unlock()
	{
		locked.clear(std::memory_order_release);
	}

private:
	const uint32_t maxLockAttempt;
};

} // namespace leo