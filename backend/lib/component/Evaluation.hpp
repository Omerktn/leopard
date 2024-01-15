#pragma once

#include <common/Chrono.hpp>

#include <limits>

namespace leo
{

struct EvaluationContext
{
	Nanoseconds currentTime;
};

struct EvaluationPreference
{
	static constexpr auto AS_BUSY_AS_POSSIBLE = Nanoseconds{0};
	static constexpr auto NEVER = Nanoseconds{std::numeric_limits<Nanoseconds::rep>::max()};

	Nanoseconds period;
};

}; // namespace leo