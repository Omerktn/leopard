#pragma once

#include "Types.hpp"

#include <chrono>
#include <ctime>
#include <string_view>

namespace leo
{

using Timestamp32 = int64_t;
using Timestamp64 = int64_t;

using Hours = std::chrono::duration<Timestamp32, std::ratio<3600>>;
using Minutes = std::chrono::duration<Timestamp32, std::ratio<60>>;
using Seconds = std::chrono::duration<Timestamp32>;
using Milliseconds = std::chrono::duration<Timestamp32, std::milli>;
using Microseconds = std::chrono::duration<Timestamp64, std::micro>;
using Nanoseconds = std::chrono::duration<Timestamp64, std::nano>;

class Clock
{
public:
	static Nanoseconds now()
	{
		struct timespec time;

		clock_gettime(CLOCK_REALTIME, &time);

		return Seconds{time.tv_sec} + Nanoseconds{time.tv_nsec};
	}

	static Nanoseconds getEvalTime()
	{
		return getSingleton().lastEvalTime;
	}

	static void setEvalTime(Nanoseconds time)
	{
		getSingleton().lastEvalTime = time;
	}

private:
	Clock() = default;

	static Clock& getSingleton()
	{
		static Clock object;
		return object;
	}

	Nanoseconds lastEvalTime = now();
};

class ScopeTimer
{
public:
	ScopeTimer(const std::string_view msg = "Time")
		: msg{msg}
		, startTime{Clock::now()}
	{}

	~ScopeTimer()
	{
		const auto timeDiffNs = Clock::now() - startTime;
		// LogFatal() << msg << ": " << static_cast<float>(timeDiffNs.count()) / 1000.0f << " us";
	}

private:
	const std::string_view msg;
	const Nanoseconds startTime;
};

template <typename NewTimeT, typename GivenTimeT>
NewTimeT durationCast(const GivenTimeT& time)
{
	return std::chrono::duration_cast<NewTimeT>(time);
}

} // namespace leo