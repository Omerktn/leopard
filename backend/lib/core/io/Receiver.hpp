#pragma once

#include <core/io/AnyEvent.hpp>

#include <functional>
#include <typeinfo>

namespace leo::core::io
{

class ReceiverDefinition
{
	using Callback = std::function<void(const core::io::AnyEvent&)>;

public:
	struct Slot
	{
		const std::type_info& eventTypeInfo;
		std::string name;
		std::string eventName;
		Callback callback;
	};

public:
	template <class... Slots>
	ReceiverDefinition(Slots&&... slotElements)
		: slots{slotElements...}
	{
		static_assert(std::conjunction_v<std::is_same<Slot, Slots>...>);
	}

	std::vector<Slot> slots{};
};

} // namespace leo::core::io