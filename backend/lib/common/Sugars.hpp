#pragma once

#include <type_traits>

namespace leo
{

template <typename T>
constexpr decltype(auto) castToUnderlying(T val)
{
	static_assert(std::is_enum_v<T>);
	using UnderlyingType = std::underlying_type_t<T>;
	return static_cast<UnderlyingType>(val);
}

} // namespace leo