#pragma once

#include <common/Sugars.hpp>
#include <common/TypeTraits.hpp>
#include <common/concurrent/adaptors/BufferedQueueAdaptor.hpp>

#include <logger/LogLevel.hpp>
#include <logger/server/Server.hpp>
#include <logger/user/LogEvents.hpp>

#include <string_view>

namespace leo::logger
{

class Logger
{
	static constexpr auto BUFFER_CAPACTIY = 4096u;
	static constexpr auto AUTO_FLUSH_THRESHOLD = 0.75f;

public:
	Logger(Server& server, LogLevel logLevel = LogLevel::DEBUG)
		: server{server}
		, bufferedQueueAdaptor{server.getQueue(), BUFFER_CAPACTIY}
		, activeLevel{logLevel}
		, seqNum{0}
	{}

	Logger(Logger&&) = delete;
	Logger(const Logger&) = delete;
	Logger& operator=(Logger&&) = delete;
	Logger& operator=(const Logger&) = delete;

	~Logger() = default;

	template <typename Event>
	void logEvent(const Event& event)
	{
		bufferedQueueAdaptor.putAll(protocol::Header::create<Event>(seqNum, Clock::now()), event);
		++seqNum;

		doAutoFlush();
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

	void flush()
	{
		bufferedQueueAdaptor.flush();
	}

private:
	template <typename... Args>
	void logTextMessage(LogLevel level, const std::string_view formatString, Args&&... args)
	{
		if (castToUnderlying(activeLevel) < castToUnderlying(level))
		{
			return;
		}

		static constexpr auto PARAM_COUNT = sizeof...(Args);

		bufferedQueueAdaptor.putAll(
			protocol::Header::create<log::special::FormattedText>(seqNum, Clock::now()),
			log::special::FormattedText{formatString, PARAM_COUNT});

		logParams(std::forward<Args>(args)...);

		++seqNum;

		doAutoFlush();
	}

	void logParams() {}

	template <typename T, typename... Args>
	void logParams(T&& value, Args&&... args)
	{
		if constexpr (std::is_bounded_array_v<std::remove_reference_t<T>>)
		{
			bufferedQueueAdaptor.putAll(log::special::FormatParameter::create<const char*>(),
										static_cast<const char*>(value));
		}
		else
		{
			bufferedQueueAdaptor.putAll(log::special::FormatParameter::create<T>(),
										std::forward<T>(value));
		}

		logParams(std::forward<Args>(args)...);
	}

	void doAutoFlush()
	{
		if (bufferedQueueAdaptor.getFullnessRatio() >= AUTO_FLUSH_THRESHOLD)
		{
			bufferedQueueAdaptor.flush();
		}
	}

private:
	Server& server;
	concurrent::BufferedQueueAdaptor<concurrent::BufferQueue> bufferedQueueAdaptor;
	LogLevel activeLevel;
	protocol::SequenceNumber seqNum;
};

} // namespace leo::logger