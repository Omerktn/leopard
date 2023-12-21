#pragma once

#include <common/Sugars.hpp>

#include <logger/user/LogEvents.hpp>

#include <string_view>

namespace leo::logger
{

enum class LogLevel : uint8_t
{
	ERROR = 0,
	WARN = 1,
	INFO = 2,
	DEBUG = 3
};

class Logger
{
public:
	Logger(LogLevel logLevel = LogLevel::DEBUG)
		: activeLevel{logLevel}
	{}

	Logger(Logger&&) = delete;
	Logger(const Logger&) = delete;
	Logger& operator=(Logger&&) = delete;
	Logger& operator=(const Logger&) = delete;

	~Logger() = default;

	template <typename Event>
	void logEvent(const Event& event)
	{
		//
	}

	template <size_t N, typename... Args>
	void logError(const char (&formatString)[N], Args&&... args)
	{
		logTextMessage(LogLevel::ERROR, formatString, std::forward<Args>(args)...);
	}

	template <size_t N, typename... Args>
	void logWarn(const char (&formatString)[N], Args&&... args)
	{
		logTextMessage(LogLevel::WARN, formatString, std::forward<Args>(args)...);
	}

	template <size_t N, typename... Args>
	void logInfo(const char (&formatString)[N], Args&&... args)
	{
		logTextMessage(LogLevel::INFO, formatString, std::forward<Args>(args)...);
	}

	template <size_t N, typename... Args>
	void logDebug(const char (&formatString)[N], Args&&... args)
	{
		logTextMessage(LogLevel::DEBUG, formatString, std::forward<Args>(args)...);
	}

	LogLevel getCurrentLevel() const
	{
		return activeLevel;
	}

private:
	template <size_t N, typename... Args>
	void logTextMessage(LogLevel level, const char (&formatString)[N], Args&&... args)
	{
		if (castToUnderlying(activeLevel) < castToUnderlying(level))
		{
			return;
		}

		const std::string_view formatStringSv(formatString, N - 1);
	}

private:
	LogLevel activeLevel;
};

} // namespace leo::logger