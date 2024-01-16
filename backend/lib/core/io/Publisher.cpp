#include "Publisher.hpp"

#include <component/Component.hpp>

#include <algorithm>

namespace leo::core::io
{

Publisher::Publisher(const std::string_view name,
					 const std::string_view eventName,
					 const std::type_info& eventTypeInfo)
	: name{name}
	, eventName{eventName}
	, eventTypeInfo{eventTypeInfo}
	, listeners{}
{}

void Publisher::rename(const std::string_view newName)
{
	name = newName;
}

void Publisher::addListener(CompId compId, Component& component, InputIndex targetInputIdx)
{
	listeners.emplace_back(compId, std::ref(component), targetInputIdx);
}

void Publisher::removeListener(CompId compId)
{
	listeners.erase(std::remove_if(listeners.begin(),
								   listeners.end(),
								   [compId](const Listener& listener) {
									   return listener.componentId == compId;
								   }),
					listeners.end());
}

bool Publisher::doesPublishEvent(const std::type_info& otherTypeInfo) const
{
	return eventTypeInfo == otherTypeInfo;
}

void Publisher::publishImpl(const AnyEvent& eventVariant)
{
	for (auto& listener : listeners)
	{
		listener.component.get().handleAnyInput(listener.targetIdx, eventVariant);
	}
}

} // namespace leo::core::io
