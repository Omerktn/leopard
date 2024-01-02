#pragma once

#include <sstream>

namespace leo
{

template <typename T>
std::string toString(const T& val)
{
	std::stringstream ss;
	ss << val;
	return ss.str();
}

} // namespace leo