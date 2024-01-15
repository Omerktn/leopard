#pragma once

#include <concepts>
#include <ostream>

namespace leo
{

template <std::integral Underlying>
class StrongType
{
public:
	using UnderlyingType = Underlying;

public:
	explicit StrongType()
		: underying{}
	{}

	explicit StrongType(Underlying val)
		: underying{val}
	{}

	StrongType(StrongType&&) = default;
	StrongType(const StrongType&) = default;
	StrongType& operator=(StrongType&&) = default;
	StrongType& operator=(const StrongType&) = default;
	~StrongType() = default;

	explicit operator StrongType() const
	{
		return underying;
	}

	Underlying& value()
	{
		return underying;
	}

	Underlying value() const
	{
		return underying;
	}

	bool operator==(StrongType other) const
	{
		return this->value() == other.value();
	}

	bool operator<(StrongType other) const
	{
		return this->value() < other.value();
	}

	bool operator>(StrongType other) const
	{
		return this->value() > other.value();
	}

private:
	Underlying underying;
};

template <std::integral Underlying>
std::ostream& operator<<(std::ostream& os, StrongType<Underlying> strongValue)
{
	os << strongValue.value();
	return os;
}

} // namespace leo