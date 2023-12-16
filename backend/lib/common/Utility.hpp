#pragma once

#include <array>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

namespace leo
{

template <std::integral T>
void printChar(T value, char* dest, size_t size, auto... format_args)
{
	auto [_, errorCode] = std::to_chars(dest, size, value, format_args...);

	if (errorCode != std::errc())
	{
		throw std::runtime_error("Cannot convert to char. Error: " +
								 std::make_error_code(errorCode).message());
	}
}

} // namespace leo