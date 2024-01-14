#pragma once

#include <common/Chrono.hpp>

namespace leo
{

struct EvaluationContext
{
	Nanoseconds currentTime;
};

struct EvaluationPreference
{
	static constexpr auto AS_BUSY_AS_POSSIBLE = Nanoseconds{0};

	Nanoseconds period;
};

}; // namespace leo