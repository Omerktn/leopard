#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace leo::logger::formatted_text
{

std::string formatString(const std::string_view formatStr,
						 std::vector<std::string>::const_iterator valuesBegin,
						 std::vector<std::string>::const_iterator valuesEnd);

} // namespace leo::logger::formatted_text