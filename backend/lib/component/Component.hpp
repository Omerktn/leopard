#pragma once

#include <common/Types.hpp>

#include <core/io/AnyEvent.hpp>
#include <core/io/Publisher.hpp>

#include <cstdint>

namespace leo
{

class Component
{
	using Publisher = core::io::Publisher<Component>;

public:
	Component(CompId compId)
		: id{compId}
		, publisher{}
	{}

	virtual ~Component() = default;

	virtual void evaluate() = 0;
	// virtual void getPublisher() = 0;
	// virtual void getComponentDefs() = 0;

	virtual void handleAnyInput(const core::io::AnyEvent&) = 0;

	Publisher& getPublisher()
	{
		return publisher;
	}

private:
	const CompId id;
	Publisher publisher;
};

} // namespace leo
