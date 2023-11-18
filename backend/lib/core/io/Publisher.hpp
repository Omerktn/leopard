#pragma once

//#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>

#include <functional>
#include <iostream>
#include <memory>

class Component;

namespace leo::core::io
{

template <typename ComponentT>
class Publisher
{
	using CompPtrRef = std::reference_wrapper<std::unique_ptr<ComponentT>>;

	struct Listener
	{
		Listener(CompId compId, CompPtrRef comp)
			: componentId{compId}
			, component{comp}
		{}

		~Listener() = default;

		CompId componentId;
		CompPtrRef component;
	};

public:
	Publisher()
	//: listeners{}
	{}

	~Publisher() = default;

	void addListener(CompId compId, std::unique_ptr<ComponentT>& component)
	{
		listeners.emplace_back(compId, std::ref(component));
	}

	template <typename Event>
	void publish(Event&& event)
	{
		std::cout << "Publisher::publish() Event: " << event.NAME << std::endl;

		const auto eventVariant = AnyEvent{std::forward<Event>(event)};

		for (auto& listener : listeners)
		{
			listener.component.get().get()->handleAnyInput(eventVariant);
		}
	}

private:
	std::vector<Listener> listeners{};
};

} // namespace leo::core::io
