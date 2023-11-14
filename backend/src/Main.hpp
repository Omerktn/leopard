#pragma once

#include <core/Core.hpp>

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

		std::cout << "Starting " << cores.size() << " cores.\n";

		for (auto& core : cores)
		{
			threads.emplace_back([&core, this]() { core.run(this->quit); });
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}

private:
	[[maybe_unused]] int argc;
	[[maybe_unused]] char** argv;

	bool quit = false;
	std::list<core::Core> cores{};
};

} // namespace leo