#pragma once

#include <common/Types.hpp>

namespace leo::events
{

struct BboUpdate
{
	static constexpr auto EVENT_ID = 1u;
	static constexpr auto NAME = "BboUpdate";

	Bbo bbo;
};

} // namespace leo::events