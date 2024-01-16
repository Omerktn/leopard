#pragma once

#include <concepts>
#include <ostream>

namespace leo
{

namespace detail
{

template <typename Underlying, class Tag>
class StrongType
{
public:
	using UnderlyingType = Underlying;

public:
	constexpr explicit StrongType()
		: underlying{}
	{}

	template <typename... Args>
	constexpr StrongType(Args&&... args)
		: underlying{std::forward<Args>(args)...}
	{}

	constexpr explicit StrongType(const Underlying& val)
		: underlying{val}
	{}

	constexpr explicit StrongType(Underlying&& val)
		: underlying{std::move(val)}
	{}

	StrongType(const StrongType&) = default;
	StrongType& operator=(StrongType&&) = default;
	StrongType& operator=(const StrongType&) = default;
	~StrongType() = default;

	constexpr explicit operator Underlying() const
	{
		return underlying;
	}

	constexpr Underlying& value()
	{
		return underlying;
	}

	constexpr const Underlying& value() const
	{
		return underlying;
	}

private:
	Underlying underlying;
};

template <std::integral Underlying, class Tag>
std::ostream& operator<<(std::ostream& os, StrongType<Underlying, Tag> strongValue)
{
	os << strongValue.value();
	return os;
}

template <std::integral Underlying, class Tag>
class StrongIntegral : public StrongType<Underlying, Tag>
{
	using Base = StrongType<Underlying, Tag>;

public:
	using Base::Base;
	using Base::value;

	bool operator==(StrongIntegral other) const
	{
		return this->value() == other.value();
	}

	bool operator<(StrongIntegral other) const
	{
		return this->value() < other.value();
	}

	bool operator>(StrongIntegral other) const
	{
		return this->value() > other.value();
	}

	StrongIntegral& operator+=(const StrongIntegral& other)
	{
		this->value += other.value;
		return *this;
	}

	StrongIntegral operator+(const StrongIntegral& other) const
	{
		auto thisCopy = *this;
		thisCopy.value += other.value;
		return thisCopy;
	}
};

template <std::integral Underlying, class Tag>
std::ostream& operator<<(std::ostream& os, StrongIntegral<Underlying, Tag> strongValue)
{
	os << strongValue.value();
	return os;
}

} // namespace detail

#define DEFINE_STRONG_TYPE(NAME, UNDERLYING)                                                       \
	using NAME = detail::StrongType<UNDERLYING, class Tag_NAME>;

#define DEFINE_STRONG_INT(NAME, UNDERLYING)                                                        \
	using NAME = detail::StrongIntegral<UNDERLYING, class Tag_NAME>;

} // namespace leo

namespace std
{

template <std::integral Underlying, class Tag>
struct hash<leo::detail::StrongIntegral<Underlying, Tag>>
{
	std::size_t
	operator()(const leo::detail::StrongIntegral<Underlying, Tag>& strongValue) const noexcept
	{
		return std::hash(strongValue.value());
	}
};

} // namespace std