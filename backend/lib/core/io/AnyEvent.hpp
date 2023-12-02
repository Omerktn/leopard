#pragma once

#include <common/Types.hpp>
#include <common/Variant.hpp>
#include <core/events/BboUpdate.hpp>
#include <core/events/SayHi.hpp>

#include <variant>

namespace leo::core::io
{

//std::monostate
using AnyEvent = std::variant<events::BboUpdate, events::SayHi>;

template <typename Event>
constexpr EventId getEventId()
{
	return getIndex<AnyEvent, Event>::value;
}

template <auto F, typename T>
auto delegateHandler(T& obj)
{
	return [&obj](const AnyEvent& arg) { return (obj.*F)(arg); };
}

} // namespace leo::core::io
