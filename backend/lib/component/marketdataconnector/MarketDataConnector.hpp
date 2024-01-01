#pragma once

#include <common/Types.hpp>
#include <component/Component.hpp>
#include <core/events/BboUpdate.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>

namespace leo::mdc
{

class MarketDataConnector : public Component
{
	using Base = Component;

public:
	enum PublisherKind : PublisherIndex
	{
		BBO = 0,
		SAY_HI = 1
	};

public:
	MarketDataConnector(CompId compId, logger::Logger&& compLogger)
		: Base{compId,
			   std::move(compLogger),
			   core::io::PublisherSchema(core::io::Publisher::create<events::BboUpdate>("Bbo-Out"),
										 core::io::Publisher::create<events::SayHi>("SayHi-Out")),
			   {}}
	{
		bboUpdate.bbo.bid = Price{Price::FromDouble{}, 100.1};
		bboUpdate.bbo.ask = Price{Price::FromDouble{}, 120.2};
		bboUpdate.bbo.bidQty = Quantity(Quantity::FromDouble{}, 75.25);
		bboUpdate.bbo.askQty = Quantity(Quantity::FromDouble{}, 133.0);
	}

	~MarketDataConnector()
	{
		std::cout << "MDC::~MarketDataConnector()" << std::endl;
	}

	virtual void evaluate() override
	{
		static constexpr auto INCREMENT = Price{Price::FromDouble{}, 0.5};
		bboUpdate.bbo.bid += INCREMENT;
		bboUpdate.bbo.ask += INCREMENT;

		//std::cout << "\nMDC::evaluate() >>> Publishing: " << bboUpdate.bbo << "\n";
		logger.logInfo("Publishing bbo: ", bboUpdate.bbo.bid.getAsDouble());
		logger.flush();
		Base::getPublisher(PublisherKind::BBO).publish(bboUpdate);
	}

private:
	events::BboUpdate bboUpdate{};
};

} // namespace leo::mdc
