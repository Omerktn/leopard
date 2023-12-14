#pragma once

#include <common/concurrent/BufferQueue.hpp>
#include <logger/user/FullMessage.hpp>

#include <chrono>
#include <iostream> // rm?
#include <thread>

namespace leo::logger
{

class Server
{
public:
	Server() {}

	void run()
	{
		using namespace std::chrono_literals;

		std::cout << "Logger Server is starting" << std::endl;

		while (true)
		{
			auto& buffer = queue.takeBuffer();

			const auto hasAnyData = buffer.getReadableSize() > 0;

			if (buffer.getReadableSize() > 0)
			{
				std::cout << "[SERVER] Received " << buffer.getReadableSize() << " bytes:\n";

				const auto& header = buffer.get<logger::protocol::Header>();

				std::cout << "[Seq = " << header.sequenceNumber << "][ EventId = " << header.eventId
						  << "]\n";
			}

			/*if (count > 0)
			{
				std::cout << "[SERVER] Received " << count << " messages:\n";
				for (auto it = begin; it != end; it++)
				{
					const auto& fullMsg = *it;
					const auto* arbEv2 = std::get_if<log::ArbitraryEvent2>(&fullMsg.variant);
					if (arbEv2)
					{
						std::cout << "[ " << fullMsg.userId << " ][ " << arbEv2->EVENT_NAME
								  << " ][ " << arbEv2->msg.toStringView() << " ]\n";
					}
					//std::cout << "[MSG] = \"" << *it << "\"\n";
				}
				std::cout << std::flush;
			}*/

			if (!hasAnyData)
			{
				std::this_thread::sleep_for(10ms);
			}
		}
	}

	concurrent::BufferQueue& getQueue()
	{
		return queue;
	}

private:
	concurrent::BufferQueue queue{1024};
};

}; // namespace leo::logger