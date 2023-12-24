#pragma once

#include <common/Chrono.hpp>
#include <common/Types.hpp>

#include <logger/user/LogEvents.hpp>

#include <chrono>
#include <type_traits>
#include <variant>

namespace leo::logger
{

namespace protocol
{

using SequenceNumber = uint16_t;
using EventId = uint16_t;

struct Header
{
public:
	template <typename Event>
	static Header create(SequenceNumber sequenceNumber, Nanoseconds timestamp)
	{
		static_assert(ContainsTypeV<Event, AllLogEvents>,
					  "Your parameter type is not in the list.");
		return Header{sequenceNumber, IndexOfV<Event, AllLogEvents>, timestamp};
	}

private:
	explicit Header(SequenceNumber sequenceNumber, EventId eventId, Nanoseconds timestamp)
		: sequenceNumber{sequenceNumber}
		, eventId{eventId}
		, timestamp{timestamp.count()}
	{}

public:
	SequenceNumber sequenceNumber;
	EventId eventId;
	std::chrono::nanoseconds::rep timestamp;
} __attribute__((packed));
static_assert(std::is_standard_layout_v<Header>);

} // namespace protocol

}; // namespace leo::logger