#pragma once

#include <component/marketdataconnector/Config.hpp>

#include <common/Types.hpp>
#include <component/Component.hpp>
#include <core/events/BboUpdate.hpp>

#include <logger/user/FreeLogger.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>

namespace leo::mdc
{

class MarketDataConnector : public Component
{
	using Base = Component;

	static constexpr auto PUBLISH_INTERVAL = Milliseconds{750};

public:
	enum PublisherKind : PublisherIndex::UnderlyingType
	{
		BBO = 0,
		SAY_HI = 1
	};

public:
	MarketDataConnector(core::Core& core, CompId compId, logger::Logger&& compLogger)
		: Base{core,
			   compId,
			   std::move(compLogger),
			   config.getFieldsDefinition(),
			   core::io::PublisherSchema(core::io::Publisher::create<events::BboUpdate>("Bbo-Out"),
										 core::io::Publisher::create<events::SayHi>("SayHi-Out")),
			   {}}
	{
		bboUpdate.bbo.bid = Price{Price::UnderlyingType::FromDouble{}, 100.1};
		bboUpdate.bbo.ask = Price{Price::UnderlyingType::FromDouble{}, 120.2};
		bboUpdate.bbo.bidQty = Quantity(Quantity::UnderlyingType::FromDouble{}, 75.25);
		bboUpdate.bbo.askQty = Quantity(Quantity::UnderlyingType::FromDouble{}, 133.0);

		this->evalPreference = EvaluationPreference{.period = Milliseconds{750}};
	}

	~MarketDataConnector()
	{
		std::cout << "MDC::~MarketDataConnector()" << std::endl;
	}

	virtual bool applyConfig(ConfigFieldId fieldId,
							 const component::config::FieldValue& valueVar) override
	{
		Config newConfig{config};
		newConfig.setField(fieldId, valueVar);

		const auto success = !Config::validate2(newConfig).has_value();
		if (success)
		{
			config = newConfig;
		}
		return success;
	}

	virtual void evaluate(const EvaluationContext& evalContext) override
	{
		/*if (evalContext.currentTime < lastTimePublished + PUBLISH_INTERVAL)
		{
			return;
		}*/
		lastTimePublished = evalContext.currentTime;

		static constexpr auto INCREMENT = Price{Price::UnderlyingType::FromDouble{}, 0.5};
		bboUpdate.bbo.bid.value() += INCREMENT.value();
		bboUpdate.bbo.ask.value() += INCREMENT.value();

		//std::cout << "\nMDC::evaluate() >>> Publishing: " << bboUpdate.bbo << "\n";
		logger.logInfo("Publishing: {}", bboUpdate.bbo);

		LOG_INFO("Deneme error!! {}", 32);

		Base::getPublisher(PublisherKind::BBO).publish(bboUpdate);
	}

private:
	Config config;
	events::BboUpdate bboUpdate{};
	Nanoseconds lastTimePublished{};
};

} // namespace leo::mdc
