#pragma once

#include <common/StringOperations.hpp>

#include <logger/formattedText/FormattedTextEvent.hpp>

#include <cmath>
#include <concepts>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

namespace leo::logger::formatted_text
{

template <typename T>
void serialize(std::string& out, const T& val);

template <typename T>
requires std::is_integral_v<T>
void serialize(std::string& out, const T& val)
{
	out = std::to_string(val);
}

template <typename T>
requires std::is_floating_point_v<T>
void serialize(std::string& out, const T& val)
{
	std::ostringstream oss;
	oss << std::setprecision(12) << std::noshowpoint << val;
	out = oss.str();
}

template <>
void serialize<const char*>(std::string& out, const char* const& val)
{
	out = val;
}

// For everything else

template <typename T>
void serialize(std::string& out, const T& val)
{
	out = toString(val);
}

} // namespace leo::logger::formatted_text