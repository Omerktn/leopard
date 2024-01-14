#pragma once

#include <common/Chrono.hpp>
#include <common/concurrent/BufferQueue.hpp>

#include <logger/LogLevel.hpp>
#include <logger/server/Decoder.hpp>
#include <logger/user/LogFields.hpp>
#include <logger/user/Protocol.hpp>

#include <mutex>
#include <optional>
#include <ostream>
#include <sstream>
#include <unordered_map>

namespace leo::logger
{

class Server
{
public:
	using UserId = uint32_t;

	static constexpr auto LINES_BUFFER_TIME = Milliseconds{300};

private:
	struct User
	{
		explicit User(UserId userId, const std::string& name, bool isFreeLogger);
		explicit User(UserId userId, bool isFreeLogger);

		UserId id;
		std::optional<std::string> name;
		bool isFreeLogger;
		concurrent::BufferQueue queue;

		protocol::SequenceNumber sequenceNumber{0};
		Nanoseconds lastTimeRead = Nanoseconds{0};
		bool hadDataLastTime = true;
	};

	class LineContainer
	{
		struct Line
		{
			Nanoseconds timestamp;
			std::string text;

			bool operator<(const Line& other) const
			{
				return this->timestamp < other.timestamp;
			}
		};

	public:
		using Lines = std::vector<Line>;
		using ConstIterator = Lines::const_iterator;

	public:
		LineContainer() = default;

		void append(Nanoseconds timestamp, std::string&& text);

		ConstIterator begin() const;
		ConstIterator end() const;

		bool empty() const;
		void clear();

	private:
		Lines lines{};
	};

	static constexpr auto MIN_WAIT_BEFORE_READ = Milliseconds{1};

public:
	Server();
	~Server();

	void run();

	UserId registerUser(const std::string& name, bool isFreeLogger);
	void unregisterUser(UserId);

	concurrent::BufferQueue& getUserQueue(UserId);

	void flushLinesIfTimeOut(Nanoseconds currentTime);

private:
	friend Decoder::Impl;

	bool checkAndSetSequenceNumber(protocol::SequenceNumber seqNum);

	template <typename Event>
	void handleEvent(const protocol::Header& header, const Event& event)
	{
		const auto serverEvent = typename Event::SERVER_SIDE_EVENT{event};

		const auto timestamp = Nanoseconds{header.timestamp};

		std::ostringstream oss;
		writeEventFields(serverEvent.EVENT_NAME, timestamp, serverEvent.getFields(), oss);
		lineContainer.append(timestamp, oss.str());
	}

	void writeEventFields(const std::string_view eventName,
						  Nanoseconds timestamp,
						  const logger::Fields& fields,
						  std::ostream& out);

	void handleText(const protocol::Header& header,
					LogLevel level,
					const std::string_view formatStr,
					std::vector<std::string>::const_iterator paramsBegin,
					std::vector<std::string>::const_iterator paramsEnd);
	void writeText(LogLevel level,
				   Nanoseconds timestamp,
				   const std::string_view text,
				   bool hasValidFormat,
				   std::ostream& out);

private:
	std::mutex userMutex{};
	std::unordered_map<UserId, User> users{};
	Decoder decoder{*this};
	User* currentDecodedUser{nullptr};

	LineContainer lineContainer{};

	UserId nextUserId{1};
};

}; // namespace leo::logger