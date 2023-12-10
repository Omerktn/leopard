#pragma once

#include <common/concurrent/Queue.hpp>

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
			const auto [begin, end] = queue.takeAll();

			const auto count = std::distance(begin, end);

			if (count > 0)
			{
				std::cout << "[SERVER] Received " << count << " message:\n";
				for (auto it = begin; it != end; it++)
				{

					std::cout << "[MSG] = \"" << *it << "\"\n";
				}
				std::cout << std::flush;
			}

			std::this_thread::sleep_for(10ms);
		}
	}

	concurrent::Queue<std::string>& getQueue()
	{
		return queue;
	}

private:
	concurrent::Queue<std::string> queue{1024};
};

}; // namespace leo::logger