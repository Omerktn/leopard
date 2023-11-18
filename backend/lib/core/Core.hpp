#pragma once

#include <common/Types.hpp>
#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>

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
	explicit Core(CoreId coreId);
	~Core() = default;

	void run(const bool& quit);

private:
	const CoreId id;
	io::AnyEvent eventVariant;
	std::vector<std::unique_ptr<leo::Component>> components;
};

} // namespace leo::core
