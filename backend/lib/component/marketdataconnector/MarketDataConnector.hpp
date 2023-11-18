#pragma once

#include <common/Types.hpp>
#include <component/Component.hpp>
#include <core/events/BboUpdate.hpp>
#include <core/io/IoSchema.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>

namespace leo::mdc
{

class MarketDataConnector : public Component
{
	using Base = Component;

public:
	//using OutputSchema = core::io::IoSchema<events::BboUpdate>;
	//using Publisher = core::io::Publisher<OutputSchema>;

public:
	MarketDataConnector(CompId compId)
		: Base{compId}
	{
		bboUpdate.bbo.bid = Price{Price::FromDouble{}, 100.1};
		bboUpdate.bbo.ask = Price{Price::FromDouble{}, 120.2};
	}

	~MarketDataConnector()
	{
		std::cout << "MDC::~MarketDataConnector()" << std::endl;
	}

	virtual void evaluate() override
	{
		std::cout << "MDC::evaluate() >>> Publishing BBO( " << bboUpdate.bbo.bid.getAsDouble()
				  << " / " << bboUpdate.bbo.ask.getAsDouble() << " )\n";

		static constexpr auto INCREMENT = Price{Price::FromDouble{}, 0.5};
		bboUpdate.bbo.bid += INCREMENT;
		bboUpdate.bbo.ask += INCREMENT;

		Base::getPublisher().publish(bboUpdate);
	}

	virtual void handleAnyInput(const core::io::AnyEvent&) override
	{
		assert(!"Not implemented");
	}

private:
	events::BboUpdate bboUpdate{};
};

} // namespace leo::mdc
