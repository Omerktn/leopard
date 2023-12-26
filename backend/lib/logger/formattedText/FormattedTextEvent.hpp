#pragma once

#include <common/FixedString.hpp>
#include <common/Sugars.hpp>
#include <common/Types.hpp>
#include <logger/user/LogFields.hpp>

#include <type_traits>
#include <variant>

namespace leo::log::special
{

using FormattedParameterTypes = TypeList<const char*,
										 double,
										 float,
										 bool,
										 uint8_t,
										 int8_t,
										 uint16_t,
										 int16_t,
										 uint32_t,
										 int32_t,
										 uint64_t,
										 int64_t>;

using FormattedParameterVariant = CreateVariant<FormattedParameterTypes>;

struct FormattedText
{
	explicit FormattedText(const std::string_view formatString,
						   logger::LogLevel level,
						   uint8_t paramCount)
		: formatString{formatString}
		, level{castToUnderlying(level)}
		, paramCount{paramCount}
	{}

	std::string_view formatString;
	uint8_t level;
	uint8_t paramCount;
} __attribute__((packed));
static_assert(std::is_standard_layout_v<FormattedText>);

struct FormatParameter
{
	template <typename T>
	static FormatParameter create()
	{
		static_assert(ContainsTypeV<T, FormattedParameterTypes>,
					  "Your parameter type is not in the list.");
		return FormatParameter{IndexOfV<T, FormattedParameterTypes>};
	}

private:
	FormatParameter(uint8_t typeId)
		: typeId{typeId}
	{}

public:
	uint8_t typeId;
} __attribute__((packed));
static_assert(std::is_standard_layout_v<FormatParameter>);

}; // namespace leo::log::special