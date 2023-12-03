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
	using CompPtrRef = std::reference_wrapper<std::unique_ptr<Component>>;

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
	Publisher(const std::string_view name);

	/*Publisher(const PublisherType& other) = default;
	Publisher(PublisherType&& other) = default;
	PublisherType& operator=(const PublisherType&) = default;
	PublisherType& operator=(PublisherType&&) = default;*/

	~Publisher() = default;

	void rename(const std::string_view newName);

	void
	addListener(CompId compId, std::unique_ptr<Component>& component, InputIndex targetInputIdx);

	void removeListener(CompId compId);

	template <typename Event>
	void publish(Event&& event)
	{
		std::cout << "Publisher(" << name << ")::publish Event: " << event.NAME << std::endl;

		const auto eventVariant = AnyEvent{std::forward<Event>(event)};
		publishImpl(eventVariant);
	}

private:
	void publishImpl(const AnyEvent& eventVariant);

private:
	std::string name;
	std::vector<Listener> listeners;
};

class PublisherSchema
{
public:
	struct Slot
	{
		const std::type_info& eventTypeInfo;
		std::string name;
		std::string eventName;
	};

public:
	template <class... Slots>
	PublisherSchema(Slots&&... slotElements)
		: slots{slotElements...}
	{
		static_assert(std::conjunction_v<std::is_same<Slot, Slots>...>);
	}

	std::vector<Slot> slots{};
};

} // namespace leo::core::io
