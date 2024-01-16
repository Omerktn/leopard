#pragma once

//#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>

#include <functional>
#include <iostream>
#include <memory>

namespace leo
{
class Component;
};

namespace leo::core::io
{

class Publisher
{
	using CompPtrRef = std::reference_wrapper<Component>;

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
	Publisher(const std::string_view name,
			  const std::string_view eventName,
			  const std::type_info& eventTypeInfo);

	~Publisher() = default;

	template <typename EventType>
	static Publisher create(const std::string_view name)
	{
		return Publisher{name, EventType::NAME, typeid(EventType)};
	}

	void rename(const std::string_view newName);

	void addListener(CompId compId, Component& component, InputIndex targetInputIdx);

	void removeListener(CompId compId);

	bool doesPublishEvent(const std::type_info& otherTypeInfo) const;

	template <typename Event>
	void publish(Event&& event)
	{
		if (typeid(Event) != eventTypeInfo)
		{
			throw std::runtime_error("Attempted to publish " + std::string{typeid(Event).name()} +
									 " from a Publisher of " + std::string{eventTypeInfo.name()});
		}

		//std::cout << "Publisher(" << name << ")::publish Event: " << event.NAME
		//		  << " id: " << getEventId<Event>() << std::endl;

		const auto eventVariant = AnyEvent{std::forward<Event>(event)};
		publishImpl(eventVariant);
	}

private:
	void publishImpl(const AnyEvent& eventVariant);

private:
	std::string name;
	std::string eventName;
	const std::type_info& eventTypeInfo;
	std::vector<Listener> listeners;
};

struct PublisherSchema
{
	template <class... Publishers>
	PublisherSchema(Publishers&&... slotElements)
		: publishers{slotElements...}
	{
		static_assert(std::conjunction_v<std::is_same<Publisher, Publishers>...>);
	}

	std::vector<Publisher> publishers{};
};

} // namespace leo::core::io
