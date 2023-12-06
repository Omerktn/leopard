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

Core::Core(CoreId coreId)
	: id{coreId}
	, eventVariant{}
	, components{}
{
	components.reserve(64);
}

void Core::run(const bool& quit)
{
	std::cout << "Core " << id << " is running." << std::endl;

	const auto mdcId = 7001;
	auto& mdc = components.emplace_back(std::make_unique<mdc::MarketDataConnector>(mdcId));

	const auto bfId = 7002;
	auto& bboFilter = components.emplace_back(std::make_unique<bbo_filter::BboFilter>(bfId));

	mdc->getPublisher(mdc::MarketDataConnector::PublisherKind::Bbo::INDEX)
		.addListener(bfId, bboFilter, bbo_filter::BboFilter::InputKind::BboSource::INDEX);

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
