#pragma once

#include <common/Chrono.hpp>
#include <common/TypeList.hpp>
#include <common/Types.hpp>
#include <logger/TestEvents.hpp>

#include <chrono>
#include <type_traits>
#include <variant>

namespace leo::logger
{

using AllLogEvents = TypeList<log::ArbitraryEvent1, log::ArbitraryEvent2>;

namespace protocol
{

using SequenceNumber = uint16_t;
using EventId = uint16_t;

// Disable char data[] warning
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"

struct Header
{
public:
	template <typename Event>
	static Header create(SequenceNumber sequenceNumber, Nanoseconds timestamp)
	{
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
	char data[];
} __attribute__((packed));
static_assert(std::is_standard_layout_v<Header>);

#pragma clang diagnostic pop

} // namespace protocol

}; // namespace leo::logger