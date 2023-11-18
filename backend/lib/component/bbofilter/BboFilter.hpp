#pragma once

#include <common/Types.hpp>
#include <common/Variant.hpp>
#include <component/Component.hpp>
#include <core/events/BboUpdate.hpp>
#include <core/io/IoSchema.hpp>

#include <cstdint>
#include <iostream>

namespace leo::bbo_filter
{

class BboFilter : public Component
{
	using Base = Component;

public:
	//using InputSchema = core::io::IoSchema<events::BboUpdate>;

public:
	BboFilter(CompId compId)
		: Base{compId}
	{}

	~BboFilter()
	{
		std::cout << "BboFilter::~BboFilter()" << std::endl;
	}

	virtual void evaluate() override
	{
		std::cout << "BboFilter::evaluate() Evaluating BboFilter!\n";
	}

	virtual void handleAnyInput(const core::io::AnyEvent& anyEvent) override
	{
		std::visit(
			leo::compose(
				[](const events::BboUpdate& arg) { std::cout << "Received: " << arg.NAME << '\n'; },
				[](auto) { std::cout << "Received something else.\n"; }),
			anyEvent);
	}

private:
};

} // namespace leo::bbo_filter
