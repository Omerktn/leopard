#pragma once

#include <ostream>

namespace leo::logger
{

enum class LogLevel : uint8_t
{
	ERROR = 0,
	WARN = 1,
	INFO = 2,
	DEBUG = 3
};

std::ostream& operator<<(std::ostream& os, LogLevel level);

} // namespace leo::logger