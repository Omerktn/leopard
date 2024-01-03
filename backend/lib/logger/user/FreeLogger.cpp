#include "FreeLogger.hpp"

#include <cassert>

namespace leo::logger
{

FreeLogger* FreeLogger::singleton = nullptr;

void FreeLogger::initialize(Server& server)
{
	assert(!singleton);
	singleton = new FreeLogger(server, "-Free-Logger-", true, LogLevel::DEBUG);
}

} // namespace leo::logger