#pragma once

#include <common/Math.hpp>

#include <ostream>

namespace leo
{

template <size_t DECIMAL_PLACE>
struct FixedDecimal
{
	struct FromDouble
	{};

	using FixedPriceType = FixedDecimal<DECIMAL_PLACE>;
	using ValueType = int64_t;

	static constexpr auto INVALID_PRICE = std::numeric_limits<ValueType>::max();
	static constexpr auto TEN_TO_THE_POWER_OF_DECIMAL = pow(10u, DECIMAL_PLACE);

	constexpr FixedDecimal()
		: value{INVALID_PRICE}
	{}

	constexpr FixedDecimal(ValueType userValue)
		: value{userValue * TEN_TO_THE_POWER_OF_DECIMAL}
	{}

	constexpr FixedDecimal(FromDouble, double userValue)
		: value{static_cast<ValueType>(userValue * TEN_TO_THE_POWER_OF_DECIMAL)}
	{}

	constexpr bool isValid() const noexcept
	{
		return value == INVALID_PRICE;
	}

	constexpr double getAsDouble() const
	{
		return static_cast<double>(value) / TEN_TO_THE_POWER_OF_DECIMAL;
	}

	constexpr ValueType getValue() const
	{
		return value;
	}

	FixedPriceType operator+(const FixedPriceType& rhs) const noexcept
	{
		FixedDecimal newPrice = *this;
		newPrice.value += rhs.getValue();
		return newPrice;
	}

	FixedPriceType operator-(const FixedPriceType& rhs) const noexcept
	{
		FixedDecimal newPrice = *this;
		newPrice.value -= rhs.getValue();
		return newPrice;
	}

	void operator+=(const FixedPriceType& rhs) noexcept
	{
		this->value += rhs.getValue();
	}

	void operator-=(const FixedPriceType& rhs) noexcept
	{
		this->value -= rhs.getValue();
	}

	bool operator==(const FixedPriceType& rhs) const noexcept
	{
		return this->value == rhs.getValue();
	}

	bool operator>(const FixedPriceType& rhs) const noexcept
	{
		return this->value > rhs.getValue();
	}

	bool operator>=(const FixedPriceType& rhs) const noexcept
	{
		return this->value >= rhs.getValue();
	}

	bool operator<(const FixedPriceType& rhs) const noexcept
	{
		return this->value < rhs.getValue();
	}

	bool operator<=(const FixedPriceType& rhs) const noexcept
	{
		return this->value <= rhs.getValue();
	}

	friend std::ostream& operator<<(std::ostream& os, const FixedPriceType& matrix)
	{
		return os << matrix.getAsDouble();
	}

	ValueType value;
};

} // namespace leo