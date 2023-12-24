#include "LogLevel.hpp"

namespace leo::logger
{

std::ostream& operator<<(std::ostream& os, LogLevel level)
{
	switch (level)
	{
	case LogLevel::ERROR:
		os << "ERROR";
		break;
	case LogLevel::WARN:
		os << "WARN";
		break;
	case LogLevel::INFO:
		os << "INFO";
		break;
	case LogLevel::DEBUG:
		os << "DEBUG";
		break;
	}
	return os;
}

} // namespace leo::logger