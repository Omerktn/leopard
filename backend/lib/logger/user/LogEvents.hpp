#pragma once

#include <common/TypeList.hpp>

#include <logger/TestEvents.hpp>

namespace leo::logger
{

using AllLogEvents = TypeList<log::ArbitraryEvent1, log::ArbitraryEvent2>;

}