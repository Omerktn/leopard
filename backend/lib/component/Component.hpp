#pragma once

#include <cstdint>

namespace leo
{

class Component
{
public:
	Component(int32_t compId)
		: id{compId}
	{}

	virtual ~Component() = default;

	virtual void evaluate() = 0;
	// virtual void getPublisher() = 0;
	// virtual void getComponentDefs() = 0;

private:
	const int32_t id;
};

} // namespace leo
