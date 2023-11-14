#pragma once

#include <component/Component.hpp>
#include <component/bbofilter/BboFilter.hpp>
#include <component/marketdataconnector/MarketDataConnector.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

namespace leo::core
{

class Core
{
public:
	Core(int32_t coreId)
		: id{coreId}
		, components{}
	{
		components.reserve(64);
	}

	~Core() = default;

	void run(const bool& quit)
	{
		std::cout << "Core " << id << " is running." << std::endl;

		const auto mdcId = 7001;
		components.emplace_back(std::make_unique<mdc::MarketDataConnector>(mdcId));

		const auto bfId = 7002;
		components.emplace_back(std::make_unique<bbo_filter::BboFilter>(bfId));

		while (!quit)
		{
			for (auto& component : components)
			{
				component->evaluate();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds{750});
		}
	}

private:
	const int32_t id;

	std::vector<std::unique_ptr<leo::Component>> components;
};

} // namespace leo::core
