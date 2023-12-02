#pragma once

#include <common/Functional.hpp>
#include <common/Sugars.hpp>
#include <common/Types.hpp>
#include <common/Variant.hpp>
#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>
#include <core/io/IoSchema.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>

namespace leo::bbo_filter
{

class BboFilter : public Component
{
	using Base = Component;

public:
	BboFilter(CompId compId)
		: Base{compId,
			   0,
			   Base::InputDefinition(Base::InputDefinition::Input{
				   .eventTypeInfo = typeid(events::BboUpdate),
				   .name = "BBO-Input",
				   .eventName = events::BboUpdate::NAME,
				   .callback = core::io::delegateHandler<&BboFilter::handleBboUpdate>(*this)})}
	{}

	~BboFilter()
	{
		std::cout << "BboFilter::~BboFilter()" << std::endl;
	}

	virtual void evaluate() override
	{
		std::cout << "BboFilter::evaluate() Evaluating BboFilter!\n";
	}

public:
	struct InputKind
	{
		static constexpr auto NUMBER_OF_INPUTS = 2;

		struct BboSource
		{
			static constexpr InputIndex INDEX = 0;
			static constexpr auto NAME = "BboSource";
			using EventType = events::BboUpdate;
		};
		struct HiSayer
		{
			static constexpr InputIndex INDEX = 1;
			static constexpr auto NAME = "HiSayer";
			using EventType = events::SayHi;
		};
	};

	/*virtual void handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent) override
	{
		switch (idx)
		{
		case InputKind::BboSource::INDEX:
			dispatchToHandler<InputKind::BboSource>(anyEvent);
			break;
		case InputKind::HiSayer::INDEX:
			dispatchToHandler<InputKind::HiSayer>(anyEvent);
			break;
		default:
			throw std::runtime_error("Unexpected InputIndex: " + std::to_string(idx));
		}
	}*/

private:
	void handleBboUpdate(const core::io::AnyEvent& anyEvent)
	{
		std::cout << "Received event " << anyEvent.index() << "\n";
	}

	/*template <typename InputType>
	void dispatchToHandler(const core::io::AnyEvent& anyEvent)
	{
		using ExpectedEvent = typename InputType::EventType;

		const auto* eventPtr = std::get_if<ExpectedEvent>(&anyEvent);

		if (!eventPtr)
		{
			assert(!"Variant holds the wrong Event.");
			return;
		}

		handle(InputType{}, *eventPtr);
		//std::visit(leo::compose([](const ExpectedEvent& arg) { handle(InputType{}, arg); },
		//						[](auto) { std::cout << "Received something else.\n"; }),
		//		   anyEvent);
	}

	void handle(InputKind::BboSource, const events::BboUpdate& event)
	{
		std::cout << "Received BboSource! Name: " << event.NAME << '\n';
	}

	void handle(InputKind::HiSayer, const events::SayHi& event)
	{
		std::cout << "Received HiSayer! Name: " << event.NAME << '\n';
	}*/

	virtual void initPublishers() override
	{
		using namespace std::string_literals;

		//Base::getPublisher(InputKind::BboSource::INDEX)
		//	.rename(InputKind::BboSource::NAME + " publisher"s);
		//Base::getPublisher(InputKind::HiSayer::INDEX)
		//	.rename(InputKind::HiSayer::NAME + " publisher"s);
	}

private:
};

} // namespace leo::bbo_filter
