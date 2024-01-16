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

	const auto mdcId = CompId{7001};
	auto& mdc = components.emplace_back(std::make_unique<mdc::MarketDataConnector>(
		mdcId, logger::Logger{loggerServer, "MDC", false, logger::LogLevel::DEBUG}));

	const auto bfId = CompId{7002};
	auto& bboFilter = components.emplace_back(std::make_unique<bbo_filter::BboFilter>(
		bfId, logger::Logger{loggerServer, "BBF", false, logger::LogLevel::DEBUG}));

	mdc->getPublisher(mdc::MarketDataConnector::PublisherKind::BBO)
		.addListener(bfId, bboFilter, InputIndex{static_cast<uint16_t>(0)});

	while (!quit)
	{
		const auto now = Clock::now();
		const auto evalContext = EvaluationContext{.currentTime = now};

		for (auto& component : components)
		{
			if (shouldEval(*component, now))
			{
				component->evaluate(evalContext);
				component->lastEvaluationTime = now;
			}
		}

		for (auto& component : components)
		{
			component->getLogger().flush();
		}
	}
}

bool Core::shouldEval(const Component& component, Nanoseconds currentTime)
{
	const auto period = component.getEvaluationPreference().period;

	if (period == EvaluationPreference::AS_BUSY_AS_POSSIBLE)
	{
		return true;
	}

	if (period == EvaluationPreference::NEVER)
	{
		return false;
	}

	return component.lastEvaluationTime + period < currentTime;
}

} // namespace leo::core
