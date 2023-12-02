#pragma once

//#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>

#include <functional>
#include <iostream>
#include <memory>

namespace leo::core::io
{
// TODO forward reference it?
template <typename ComponentT>
class Publisher
{
	using CompPtrRef = std::reference_wrapper<std::unique_ptr<ComponentT>>;
	using PublisherType = Publisher<ComponentT>;

	struct Listener
	{
		Listener(CompId compId, CompPtrRef comp, InputIndex targetInputIdx)
			: componentId{compId}
			, component{comp}
			, targetIdx{targetInputIdx}
		{}

		~Listener() = default;

		CompId componentId;
		CompPtrRef component;
		InputIndex targetIdx;
	};

public:
	Publisher(const std::string_view name)
		: name{name}
		, listeners{}
	{}

	/*Publisher(const PublisherType& other) {}
	Publisher(PublisherType&& other)
		: name{std::move(other.name)}
		, listener{std::move(listener)}
	{
		this->name = ;
		this->listeners = std::move(other)
	}*/

	Publisher(const PublisherType& other) = default;
	Publisher(PublisherType&& other) = default;
	PublisherType& operator=(const PublisherType&) = default;
	PublisherType& operator=(PublisherType&&) = default;

	~Publisher() = default;

	void rename(const std::string_view newName)
	{
		name = newName;
	}

	void
	addListener(CompId compId, std::unique_ptr<ComponentT>& component, InputIndex targetInputIdx)
	{
		listeners.emplace_back(compId, std::ref(component), targetInputIdx);
	}

	void removeListener(CompId compId)
	{
		std::erase(std::remove_if(listeners.begin(),
								  listeners.end(),
								  [compId](const Listener& listener) {
									  return listener.componentId == compId;
								  }),
				   listeners.end());
	}

	template <typename Event>
	void publish(Event&& event)
	{
		std::cout << "Publisher(" << name << ")::publish Event: " << event.NAME << std::endl;

		const auto eventVariant = AnyEvent{std::forward<Event>(event)};

		for (auto& listener : listeners)
		{
			listener.component.get().get()->handleAnyInput(listener.targetIdx, eventVariant);
		}
	}

private:
	std::string name;
	std::vector<Listener> listeners;
};

} // namespace leo::core::io
