#pragma once

#include <component/Component.hpp>

#include <cstdint>
#include <iostream>

namespace leo::mdc
{

class MarketDataConnector : public Component
{
	using Base = Component;

public:
	MarketDataConnector(int32_t compId)
		: Base{compId}
	{}

	~MarketDataConnector()
	{
		std::cout << "MDC::~MarketDataConnector()" << std::endl;
	}

	virtual void evaluate() override
	{
		std::cout << "MDC::evaluate() Evaluating MarketDataConnector!\n";
	}

private:
};

} // namespace leo::mdc
