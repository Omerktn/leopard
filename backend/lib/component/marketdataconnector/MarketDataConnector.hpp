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

	struct PublisherKind
	{
		static constexpr auto NUMBER_OF_PUBLISHERS = 2;

		struct Bbo
		{
			static constexpr InputIndex INDEX = 0;
			static constexpr auto NAME = "Bbo";
			using EventType = events::BboUpdate;
		};
		struct SayHi
		{
			static constexpr InputIndex INDEX = 1;
			static constexpr auto NAME = "SayHi";
			using EventType = events::SayHi;
		};
	};

public:
	MarketDataConnector(CompId compId)
		: Base{compId, PublisherKind::NUMBER_OF_PUBLISHERS}
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

		Base::getPublisher(PublisherKind::Bbo::INDEX).publish(bboUpdate);
	}

	/*virtual void handleAnyInput([[maybe_unused]] InputIndex idx, const core::io::AnyEvent&) override
	{
		assert(!"Not implemented");
	}*/

	virtual void initPublishers() override
	{
		using namespace std::string_literals;

		Base::getPublisher(PublisherKind::Bbo::INDEX)
			.rename(PublisherKind::Bbo::NAME + " publisher"s);
		Base::getPublisher(PublisherKind::SayHi::INDEX)
			.rename(PublisherKind::SayHi::NAME + " publisher"s);
	}

private:
	events::BboUpdate bboUpdate{};
};

} // namespace leo::mdc
