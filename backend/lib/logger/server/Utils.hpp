#pragma once

#include <common/Chrono.hpp>

#include <ostream>

namespace leo::logger::utils
{

void serializeTime(std::ostream& out, Nanoseconds time);

constexpr inline bool validateFormatString(const std::string_view str,
										   size_t expectedPlaceholderCount)
{
	size_t count = 0;

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == '{')
		{
			++count;

			// Check if there is a corresponding '}'
			size_t j = i + 1;
			while (j < str.size() && str[j] != '}')
			{
				++j;
			}

			if (j == str.size())
			{
				return false; // No corresponding '}'
			}

			i = j; // Move to the character after '}'
		}
	}

	return count == expectedPlaceholderCount;
}

} // namespace leo::logger::utils