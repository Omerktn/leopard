#include "Formatter.hpp"

#include <cassert>

namespace leo::logger::formatted_text
{

std::string formatString(const std::string_view formatStr,
						 std::vector<std::string>::const_iterator valuesBegin,
						 std::vector<std::string>::const_iterator valuesEnd)
{
	std::string result{formatStr};
	size_t pos = 0;

	for (auto it = valuesBegin; it != valuesEnd; ++it)
	{
		pos = result.find("{}", pos);

		if (pos != std::string::npos)
		{
			result.replace(pos, 2, *it);
		}
		else
		{
			break;
		}
	}

	[[maybe_unused]] const auto afterResult = result.find("{}", pos);
	assert(afterResult == std::string::npos);

	return result;
}

} // namespace leo::logger::formatted_text