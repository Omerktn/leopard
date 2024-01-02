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
#include <unordered_map>

namespace leo::logger
{

class Server
{
public:
	using UserId = uint32_t;

private:
	struct User
	{
		explicit User(UserId userId, const std::string& name);
		explicit User(UserId userId);

		UserId id;
		std::optional<std::string> name;
		concurrent::BufferQueue queue;

		protocol::SequenceNumber sequenceNumber{0};
		Nanoseconds lastTimeRead = Nanoseconds{0};
		bool hadDataLastTime = true;
	};

	static constexpr auto MIN_WAIT_BEFORE_READ = Milliseconds{1};

public:
	Server();
	~Server();

	void run();

	UserId registerUser(const std::string& name);
	void unregisterUser(UserId);

	concurrent::BufferQueue& getUserQueue(UserId);

private:
	friend Decoder::Impl;

	bool checkAndSetSequenceNumber(protocol::SequenceNumber seqNum);

	template <typename Event>
	void handleEvent(const protocol::Header& header, const Event& event)
	{
		const auto serverEvent = typename Event::SERVER_SIDE_EVENT{event};
		writeEventFields(serverEvent.EVENT_NAME,
						 Nanoseconds{header.timestamp},
						 serverEvent.getFields(),
						 std::cout);
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

	UserId nextUserId{1};
};

}; // namespace leo::logger