#pragma once

#include <common/TypeList.hpp>

#include <logger/TestEvents.hpp>
#include <logger/formattedText/FormattedTextEvent.hpp>

namespace leo::logger
{

using AllLogEvents = TypeList<log::special::FormattedText, //
							  log::ArbitraryEvent1,		   //
							  log::ArbitraryEvent2>;

} // namespace leo::logger