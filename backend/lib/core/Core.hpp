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
	struct ComponentItem
	{
		CompId compId;
		std::unique_ptr<Component> comp;

		bool operator==(const ComponentItem& other) const;
		bool operator<(const ComponentItem& other) const;
	};

public:
	explicit Core(CoreId coreId, std::string_view coreName, logger::Server& loggerServer);
	~Core() = default;

	void run(const std::atomic<bool>& quit);

	Component* getComponent(CompId compId);

private:
	static bool shouldEval(const Component& component, Nanoseconds currentTime);

	Component& createComponent(std::unique_ptr<Component>&& componentPtr);

private:
	const CoreId id;
	std::string name;
	logger::Server& loggerServer;
	io::AnyEvent eventVariant;
	std::vector<ComponentItem> components;
};

} // namespace leo::core
