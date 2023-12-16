#include "Utils.hpp"

#include <cstdio>
#include <ctime>

namespace leo::logger::utils
{

namespace
{

void setHumanDateTime(char* buffer, size_t bufferSize, Nanoseconds durationValue)
{
	using namespace std::chrono;

	tm time;
	system_clock::time_point timePoint{durationValue};
	time_t timeT = system_clock::to_time_t(timePoint);
	localtime_r(&timeT, &time);

	const auto nanosValue = durationValue.count() % 1'000'000;
	char nanosBuffer[7];
	snprintf(nanosBuffer, sizeof(nanosBuffer), "%06ld", nanosValue);

	snprintf(buffer,
			 bufferSize,
			 "%d-%02d-%02d %02d:%02d:%02d.%s",
			 time.tm_year + 1900,
			 time.tm_mon + 1,
			 time.tm_mday,
			 time.tm_hour,
			 time.tm_min,
			 time.tm_sec,
			 nanosBuffer);
}

} // namespace

void serializeTime(std::ostream& out, Nanoseconds time)
{
	char buf[64];
	setHumanDateTime(buf, sizeof(buf), time);
	out << buf;
}

} // namespace leo::logger::utils