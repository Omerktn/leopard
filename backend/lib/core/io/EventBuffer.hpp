/*#pragma once

#include <common/Types.hpp>

#include <array>
#include <new>
#include <type_traits>

namespace leo::core::io
{

class EventBuffer
{
public:
	static constexpr auto BUFFER_SIZE = 1024u;

public:
	EventBuffer() = default;
	~EventBuffer() = default;

template <typename Event, typename... Args>
Event* generateNew(Args&&... args)
{
	static_assert(sizeof(Event) <= BUFFER_SIZE);
	static_assert(std::is_trivially_copyable_v<Event>);

	eventId = Event::EVENT_ID;
	new (buffer.data()) Event(std::forward<Args>(args)...);
	return std::launder(reinterpret_cast<Event*>(buffer.data()));
}

template <typename Event>
Event* generateNew(Event&& event)
{
	static_assert(sizeof(Event) <= BUFFER_SIZE);
	static_assert(std::is_trivially_copyable_v<Event>);

	eventId = Event::EVENT_ID;
	new (buffer.data()) Event(std::move(event));
	return std::launder(reinterpret_cast<Event*>(buffer.data()));
}

bool isHolding(EventId id) const
{
	return eventId == id;
}

private:
EventId eventId{NAN_EVENT_ID};
std::array<std::byte, BUFFER_SIZE> buffer{};
}
;

} // namespace leo::core::io

*/