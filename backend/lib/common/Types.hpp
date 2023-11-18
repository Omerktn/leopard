#pragma once

#include <common/FixedDecimal.hpp>
#include <common/FixedString.hpp>

namespace leo
{

using CoreId = uint32_t;

using CompId = int32_t;
using ComponentName = FixedString<127>;

using EventId = uint16_t;

using Price = FixedDecimal<12>;
using Quantity = FixedDecimal<12>;

static constexpr auto NAN_EVENT_ID = std::numeric_limits<EventId>::max();

struct BboPrice
{
	Price bid{};
	Price ask{};
};

struct Bbo
{
	Price bid{};
	Quantity bidQty{};
	Price ask{};
	Quantity askQty{};

	operator BboPrice() const
	{
		return BboPrice{.bid = bid, .ask = ask};
	}
};

} // namespace leo