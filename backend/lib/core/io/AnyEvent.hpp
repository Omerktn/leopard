#pragma once

#include <common/Types.hpp>
#include <core/events/BboUpdate.hpp>
#include <core/events/SayHi.hpp>

#include <variant>

namespace leo::core::io
{

//std::monostate
using AnyEvent = std::variant<events::BboUpdate, events::SayHi>;

} // namespace leo::core::io
