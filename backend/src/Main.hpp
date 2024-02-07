#pragma once

#include <core/Core.hpp>
#include <logger/server/Server.hpp>
#include <logger/user/FreeLogger.hpp>
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
	{
		logger::FreeLogger::initialize(loggerServer);
	}

	~LeoMain() = default;

	int startProgram()
	{
		std::cout << "Program is starting...\n";

		const auto coreId = CoreId{1001u};
		cores.emplace_back(coreId, "Core1", loggerServer);

		runAllCores();

		std::cout << "All cores finished\n";

		return 0;
	}

private:
	void runAllCores()
	{
		std::thread loggerServerThread{[this]() { loggerServer.run(); }};

		std::cout << "Starting " << cores.size() << " cores.\n";

		std::vector<std::thread> threads{};
		for (auto& core : cores)
		{
			threads.emplace_back([&core, this]() { core.run(this->quit); });
		}

		runMainLoop();

		for (auto& thread : threads)
		{
			thread.join();
		}
		loggerServerThread.join();
	}

	void runMainLoop()
	{
		while (!quit.load(std::memory_order_relaxed))
		{
			localLogger.logInfo("Merhaba {}, ik ben {} jaar oud.", "Efe", 32);
			localLogger.logEvent(log::ArbitraryEvent2{"Ben de veli hehe", 19});
			localLogger.flush();
			FLUSH_FREE_LOGGER();

			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		}
	}

private:
	[[maybe_unused]] int argc;
	[[maybe_unused]] char** argv;

	std::atomic<bool> quit = false;
	logger::Server loggerServer{};
	std::list<core::Core> cores{};

	logger::Logger localLogger{loggerServer, "LeopardMain", false, logger::LogLevel::DEBUG};
};

} // namespace leo