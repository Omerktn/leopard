
#include <logger/server/Server.hpp>
#include <logger/user/Logger.hpp>

#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>

void printStatistics(const std::vector<std::chrono::nanoseconds>& durations)
{
	// Sort the durations
	std::vector<std::chrono::nanoseconds> sortedDurations = durations;
	std::sort(sortedDurations.begin(), sortedDurations.end());

	auto calculatePercentile = [&](double percentile) -> std::chrono::nanoseconds {
		if (sortedDurations.empty())
		{
			return std::chrono::nanoseconds(0);
		}

		// Calculate the index for the given percentile
		double index = percentile / 100.0 * (sortedDurations.size() - 1);
		size_t lowerIndex = static_cast<size_t>(std::floor(index));
		size_t upperIndex = static_cast<size_t>(std::ceil(index));

		// Interpolate between the two adjacent values
		auto lowerValue = sortedDurations[lowerIndex];
		auto upperValue = sortedDurations[upperIndex];
		auto interpolatedValue =
			lowerValue + (static_cast<std::chrono::nanoseconds::rep>(index) - lowerIndex) *
							 (upperValue - lowerValue);

		return interpolatedValue;
	};

	// Print percentiles
	std::cout << "Sample size: " << durations.size() << "\n";
	std::cout << "Minimum: " << sortedDurations.front().count() << " ns\n";
	std::cout << "20th Percentile: " << calculatePercentile(20).count() << " ns\n";
	std::cout << "50th Percentile (Median): " << calculatePercentile(50).count() << " ns\n";
	std::cout << "75th Percentile: " << calculatePercentile(75).count() << " ns\n";
	std::cout << "80th Percentile: " << calculatePercentile(80).count() << " ns\n";
	std::cout << "90th Percentile: " << calculatePercentile(90).count() << " ns\n";
	std::cout << "99th Percentile: " << calculatePercentile(99).count() << " ns\n";
	std::cout << "99.9th Percentile: " << calculatePercentile(99.9).count() << " ns\n";
	std::cout << "99.99th Percentile: " << calculatePercentile(99.99).count() << " ns\n";
}

int main() // int argc, char* argv[]
{
	using namespace leo;
	using namespace std::chrono_literals;
	std::cout << "Starting LOG MUTEX tool.\n";

	logger::Server loggerServer{};
	std::thread loggerServerThread{[&loggerServer]() { loggerServer.run(); }};

	logger::Logger theLogger(loggerServer, "profiler", logger::LogLevel::DEBUG);

	static constexpr auto SAMPLE_SIZE = 100'000;

	std::vector<std::chrono::nanoseconds> times{};

	std::thread testLogClient{[&theLogger, &times]() {
		while (true)
		{
			//theLogger.logInfo("Merhaba {}, ik ben {} jaar oud.", "Efe", 32);

			const auto startTime = std::chrono::steady_clock::now();
			theLogger.logEvent(log::ArbitraryEvent2{"Ben de veli hehe", 19});
			theLogger.flush();
			const auto timeTook = std::chrono::steady_clock::now() - startTime;

			times.push_back(timeTook);

			if (times.size() >= SAMPLE_SIZE)
			{
				break;
			}
		}

		std::this_thread::sleep_for(2s);

		printStatistics(times);
	}};

	loggerServerThread.join();
}