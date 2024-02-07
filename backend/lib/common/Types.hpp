#pragma once

#include <common/FixedDecimal.hpp>
#include <common/FixedString.hpp>
#include <common/StrongType.hpp>

#include <ostream>

namespace leo
{

DEFINE_STRONG_INT(CoreId, uint32_t);
DEFINE_STRONG_INT(CompId, int32_t);

DEFINE_STRONG_TYPE(ComponentName, FixedString<127>);
DEFINE_STRONG_INT(PublisherIndex, uint16_t);
DEFINE_STRONG_INT(InputIndex, uint16_t);

DEFINE_STRONG_INT(LoggerUserId, int16_t);
DEFINE_STRONG_INT(EventId, uint16_t);
DEFINE_STRONG_INT(ConfigFieldId, uint16_t);

DEFINE_STRONG_TYPE(Price, FixedDecimal<12>);
DEFINE_STRONG_TYPE(Quantity, FixedDecimal<12>);

static constexpr auto NAN_EVENT_ID = std::numeric_limits<EventId>::max();

struct BboPrice
{
	Price bid{};
	Price ask{};

	friend std::ostream& operator<<(std::ostream& os, const BboPrice& obj);
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

	friend std::ostream& operator<<(std::ostream& os, const Bbo& obj);
};

} // namespace leo