#pragma once

#include <core/Core.hpp>
#include <logger/server/Server.hpp>

#include <iostream>
#include <list>
#include <thread>
#include <vector>

namespace leo
{

class LeoMain
{
public:
	LeoMain(int argc, char** argv)
		: argc{argc}
		, argv{argv}
	{}

	~LeoMain() = default;

	int startProgram()
	{
		std::cout << "Program is starting...\n";

		const auto coreId = 1001;
		cores.emplace_back(coreId);

		runAllCores();

		std::cout << "All cores finished\n";

		return 0;
	}

private:
	void runAllCores()
	{
		std::vector<std::thread> threads{};
		threads.reserve(cores.size());

		std::thread loggerServerThread{[this]() { loggerServer.run(); }};

		std::thread testLogClient{[this]() {
			uint16_t seqNum = 0;
			while (true)
			{
				loggerServer.getQueue().putAll(
					logger::protocol::Header::create<log::ArbitraryEvent1>(seqNum, Clock::now()),
					log::ArbitraryEvent1{"Ben Omer ulan"});
				++seqNum;
				loggerServer.getQueue().putAll(
					logger::protocol::Header::create<log::ArbitraryEvent2>(seqNum, Clock::now()),
					log::ArbitraryEvent2{"Ben de veli hehe", 19});
				std::this_thread::sleep_for(std::chrono::microseconds(500'000));
				++seqNum;
			}
		}};

		std::cout << "Starting " << cores.size() << " cores.\n";

		for (auto& core : cores)
		{
			threads.emplace_back([&core, this]() { core.run(this->quit); });
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
		loggerServerThread.join();
	}

private:
	[[maybe_unused]] int argc;
	[[maybe_unused]] char** argv;

	bool quit = false;
	logger::Server loggerServer{};
	std::list<core::Core> cores{};
};

} // namespace leo