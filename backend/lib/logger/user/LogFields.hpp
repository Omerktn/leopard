#pragma once

#include <common/FixedString.hpp>

#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace leo::logger
{

using FieldVariant =
	std::variant<std::reference_wrapper<const std::string>, uint64_t, int64_t, double, bool>;
using FieldName = FixedString<31>;

using Field = std::pair<FieldName, FieldVariant>;
using Fields = std::vector<Field>;

}; // namespace leo::logger