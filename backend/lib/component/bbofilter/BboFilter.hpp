#pragma once

#include <common/Functional.hpp>
#include <common/Sugars.hpp>
#include <common/Types.hpp>
#include <common/Variant.hpp>
#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>

namespace leo::bbo_filter
{

class BboFilter : public Component
{
	using Base = Component;

public:
	BboFilter(CompId compId, logger::Logger&& compLogger)
		: Base{compId,
			   std::move(compLogger),
			   {},
			   core::io::ReceiverSchema(
				   core::io::ReceiverSchema::Slot{
					   .eventTypeInfo = typeid(events::BboUpdate),
					   .name = "BBO-In",
					   .eventName = events::BboUpdate::NAME,
					   .callback = core::io::delegateHandler<&BboFilter::handleBboUpdate>(*this)},
				   core::io::ReceiverSchema::Slot{
					   .eventTypeInfo = typeid(events::SayHi),
					   .name = "SayHi-In",
					   .eventName = events::BboUpdate::NAME,
					   .callback = core::io::delegateHandler<&BboFilter::handleSayHi>(*this)})}
	{}

	~BboFilter()
	{
		std::cout << "BboFilter::~BboFilter()" << std::endl;
	}

	virtual void evaluate(const EvaluationContext&) override
	{
		//std::cout << "BboFilter::evaluate() Evaluating BboFilter!\n";
	}

public:
private:
	void handleBboUpdate(const core::io::AnyEvent& anyEvent)
	{
		const auto& bboUpdate = std::get<events::BboUpdate>(anyEvent);
		//std::cout << "Received event: " << bboUpdate.bbo << "\n";
	}

	void handleSayHi(const core::io::AnyEvent& anyEvent)
	{
		const auto& sayHi = std::get<events::SayHi>(anyEvent);
		std::cout << "Received event: " << sayHi.NAME << "\n";
	}

private:
};

} // namespace leo::bbo_filter
