#pragma once

#include <common/Types.hpp>
#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>

#include <logger/server/Server.hpp>

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
	explicit Core(CoreId coreId, std::string_view coreName, logger::Server& loggerServer);
	~Core() = default;

	void run(const bool& quit);

private:
	static bool shouldEval(const Component& component, Nanoseconds currentTime);

private:
	const CoreId id;
	std::string name;
	logger::Server& loggerServer;
	io::AnyEvent eventVariant;
	std::vector<std::unique_ptr<leo::Component>> components;
};

} // namespace leo::core
