#pragma once

#include <common/Chrono.hpp>
#include <common/concurrent/BufferQueue.hpp>

#include <logger/LogLevel.hpp>
#include <logger/server/Decoder.hpp>
#include <logger/user/LogFields.hpp>
#include <logger/user/Protocol.hpp>

#include <ostream>

namespace leo::logger
{

class Server
{
public:
	Server();
	~Server();

	void run();

	concurrent::BufferQueue& getQueue();

private:
	friend Decoder::Impl;

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
				   std::ostream& out);

private:
	concurrent::BufferQueue queue{1024};
	Decoder decoder{*this};
};

}; // namespace leo::logger