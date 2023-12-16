#pragma once

#include <common/Chrono.hpp>

#include <ostream>

namespace leo::logger::utils
{

void serializeTime(std::ostream& out, Nanoseconds time);

} // namespace leo::logger::utils