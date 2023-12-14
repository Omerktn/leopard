#pragma once

#include <common/TypeList.hpp>
#include <common/Types.hpp>
#include <logger/TestEvents.hpp>

#include <type_traits>
#include <variant>

namespace leo::logger
{

using AllLogEvents = TypeList<log::ArbitraryEvent1, log::ArbitraryEvent2>;

namespace protocol
{

using SequenceNumber = uint16_t;
using EventId = uint16_t;

struct Header
{
public:
	template <typename Event>
	static Header create(SequenceNumber sequenceNumber)
	{
		return Header{sequenceNumber, IndexOf<Event, AllLogEvents>::value};
	}

private:
	explicit Header(SequenceNumber sequenceNumber, EventId eventId)
		: sequenceNumber{sequenceNumber}
		, eventId{eventId}
	{}

public:
	SequenceNumber sequenceNumber;
	EventId eventId;
	char data[];
} __attribute__((packed));
static_assert(std::is_standard_layout_v<Header>);

} // namespace protocol

}; // namespace leo::logger