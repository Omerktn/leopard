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

	using BufferedQueue = concurrent::BufferedQueueAdaptor<concurrent::BufferQueue>;

public:
	Logger(Server& server,
		   const std::string& name,
		   bool isFreeLogger,
		   LogLevel logLevel = LogLevel::DEBUG)
		: server{server}
		, userId{0}
		, bufferedQueue{nullptr}
		, activeLevel{logLevel}
		, seqNum{0}
	{
		userId = server.registerUser(name, isFreeLogger);
		bufferedQueue =
			std::make_unique<BufferedQueue>(server.getUserQueue(userId), BUFFER_CAPACTIY);
	}

	Logger(Logger&& other)
		: server{other.server}
		, userId{other.userId}
		, bufferedQueue{std::move(other.bufferedQueue)}
		, activeLevel{other.activeLevel}
		, seqNum{other.seqNum}
	{}

	Logger(const Logger&) = delete;
	Logger& operator=(Logger&&) = delete;
	Logger& operator=(const Logger&) = delete;

	~Logger() = default;

	template <typename Event>
	void logEvent(const Event& event)
	{
		bufferedQueue->putAll(protocol::Header::create<Event>(seqNum, Clock::now()), event);
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
		bufferedQueue->flush();
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

		bufferedQueue->putAll(
			protocol::Header::create<log::special::FormattedText>(seqNum, Clock::now()),
			log::special::FormattedText{formatString, level, PARAM_COUNT});

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
			bufferedQueue->putAll(log::special::FormatParameter::create<const char*>(),
								  static_cast<const char*>(value));
		}
		else
		{
			bufferedQueue->putAll(log::special::FormatParameter::create<T>(),
								  std::forward<T>(value));
		}

		logParams(std::forward<Args>(args)...);
	}

	void doAutoFlush()
	{
		if (bufferedQueue->getFullnessRatio() >= AUTO_FLUSH_THRESHOLD)
		{
			bufferedQueue->flush();
		}
	}

private:
	Server& server;
	Server::UserId userId;
	std::unique_ptr<BufferedQueue> bufferedQueue;
	LogLevel activeLevel;
	protocol::SequenceNumber seqNum;
};

} // namespace leo::logger