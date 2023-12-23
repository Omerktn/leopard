#pragma once

#include <core/Core.hpp>
#include <logger/server/Server.hpp>
#include <logger/user/Logger.hpp>

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

		logger::Logger theLogger(loggerServer, logger::LogLevel::DEBUG);

		std::thread testLogClient{[this, &theLogger]() {
			//uint16_t seqNum = 0;
			while (true)
			{
				/*loggerServer.getQueue().putAll(
					logger::protocol::Header::create<log::ArbitraryEvent1>(seqNum, Clock::now()),
					log::ArbitraryEvent1{"Ben Omer ulan"});
				++seqNum;
				loggerServer.getQueue().putAll(
					logger::protocol::Header::create<log::ArbitraryEvent2>(seqNum, Clock::now()),
					log::ArbitraryEvent2{"Ben de veli hehe", 19});
				++seqNum;
				loggerServer.getQueue().putAll(
					logger::protocol::Header::create<log::special::FormattedText>(seqNum,
																				  Clock::now()),
					log::special::FormattedText{
						"Hoi {}, ik ben {} jaar oud. My friend {} called me.", 3},
					log::special::FormatParameter::create<double>(),
					double{12.7},
					log::special::FormatParameter::create<uint16_t>(),
					uint16_t{35},
					log::special::FormatParameter::create<const char*>(),
					(const char*)("Hasan Huseyin"));
				++seqNum;*/

				theLogger.logInfo("Merhaba {}, ik ben {} jaar oud.", "Efe", 32);
				theLogger.flush();

				std::this_thread::sleep_for(std::chrono::microseconds(1'000'000));
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