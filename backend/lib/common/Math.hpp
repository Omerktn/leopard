#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>

namespace leo
{

template <typename T>
constexpr T pow(T num, uint32_t exp)
{
	static_assert(std::is_integral_v<T>);
	return exp == 0 ? 1 : num * pow(num, exp - 1);
}

static_assert(pow(123, 0) == 1);
static_assert(pow(123, 1) == 123);
static_assert(pow(2, 4) == 16);

} // namespace leo
