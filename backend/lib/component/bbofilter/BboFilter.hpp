#pragma once

#include <component/Component.hpp>

#include <cstdint>
#include <iostream>

namespace leo::bbo_filter
{

class BboFilter : public Component
{
	using Base = Component;

public:
	BboFilter(int32_t compId)
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

private:
};

} // namespace leo::bbo_filter
