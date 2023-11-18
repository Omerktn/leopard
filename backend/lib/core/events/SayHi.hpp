#pragma once

#include <common/Types.hpp>

namespace leo::events
{

struct SayHi
{
	static constexpr auto EVENT_ID = 2u;
	static constexpr auto NAME = "SayHi";

	uint16_t value;
};

} // namespace leo::events