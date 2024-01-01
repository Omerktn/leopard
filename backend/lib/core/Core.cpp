#include "Core.hpp"

#include <component/bbofilter/BboFilter.hpp>
#include <component/marketdataconnector/MarketDataConnector.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

namespace leo::core
{

Core::Core(CoreId coreId, std::string_view coreName, logger::Server& loggerServer)
	: id{coreId}
	, name(coreName)
	, loggerServer{loggerServer}
	, eventVariant{}
	, components{}
{
	components.reserve(64);
}

void Core::run(const bool& quit)
{
	std::cout << "Core " << id << " is running." << std::endl;

	const auto mdcId = 7001;
	auto& mdc = components.emplace_back(std::make_unique<mdc::MarketDataConnector>(
		mdcId, logger::Logger{loggerServer, "MDC", logger::LogLevel::DEBUG}));

	const auto bfId = 7002;
	auto& bboFilter = components.emplace_back(std::make_unique<bbo_filter::BboFilter>(
		bfId, logger::Logger{loggerServer, "BBF", logger::LogLevel::DEBUG}));

	mdc->getPublisher(mdc::MarketDataConnector::PublisherKind::BBO).addListener(bfId, bboFilter, 0);

	while (!quit)
	{
		for (auto& component : components)
		{
			component->evaluate();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds{750});
	}
}

} // namespace leo::core
