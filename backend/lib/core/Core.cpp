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

bool Core::ComponentItem::operator==(const ComponentItem& other) const
{
	return this->compId == other.compId;
}

bool Core::ComponentItem::operator<(const ComponentItem& other) const
{
	return this->compId < other.compId;
}

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
	auto& mdc = createComponent(std::make_unique<mdc::MarketDataConnector>(
		*this, mdcId, logger::Logger{loggerServer, "MDC", false, logger::LogLevel::DEBUG}));

	const auto bf1Id = CompId{7003};
	auto& bboFilter1 = createComponent(std::make_unique<bbo_filter::BboFilter>(
		*this, bf1Id, logger::Logger{loggerServer, "BBF1", false, logger::LogLevel::DEBUG}));

	const auto bf2Id = CompId{7002};
	auto& bboFilter2 = createComponent(std::make_unique<bbo_filter::BboFilter>(
		*this, bf2Id, logger::Logger{loggerServer, "BBF2", false, logger::LogLevel::DEBUG}));

	mdc.getPublisher(mdc::MarketDataConnector::PublisherKind::BBO)
		.addListener(bf1Id, bboFilter1, InputIndex{static_cast<uint16_t>(0)});
	mdc.getPublisher(mdc::MarketDataConnector::PublisherKind::BBO)
		.addListener(bf2Id, bboFilter2, InputIndex{static_cast<uint16_t>(0)});

	while (!quit)
	{
		const auto now = Clock::now();
		const auto evalContext = EvaluationContext{.currentTime = now};

		for (auto& item : components)
		{
			auto& component = item.comp;

			if (shouldEval(*component, now))
			{
				component->evaluate(evalContext);
				component->lastEvaluationTime = now;
			}
		}

		for (auto& item : components)
		{
			item.comp->getLogger().flush();
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

Component& Core::createComponent(std::unique_ptr<Component>&& componentPtr)
{
	ComponentItem item{.compId = componentPtr->getId(), .comp = std::move(componentPtr)};

	auto it = components.insert(std::upper_bound(components.begin(), components.end(), item),
								std::move(item));

	return *it->comp;
}

Component* Core::getComponent(CompId compId)
{
	auto it = std::lower_bound(
		components.begin(), components.end(), ComponentItem{.compId = compId, .comp = nullptr});

	if (it == components.end())
	{
		return nullptr;
	}

	return it->compId == compId ? it->comp.get() : nullptr;
}

} // namespace leo::core
