#pragma once

#include <common/Chrono.hpp>
#include <common/TypeList.hpp>
#include <common/Types.hpp>

#include <cstdint>
#include <functional>
#include <optional>
#include <string>

namespace leo::component::config
{

using SupportedTypes = TypeList<uint64_t,
								int64_t,
								uint32_t,
								int32_t,
								uint16_t,
								int16_t,
								uint8_t,
								int8_t,
								std::string,
								float,
								bool,
								MillisecondsFloat,
								Milliseconds,
								Microseconds,
								Nanoseconds>;

using FieldValue = CreateVariantT<SupportedTypes>;

struct FieldDefinition
{
	ConfigFieldId id;
	uint64_t typeIndex;
	std::string name;
	std::function<void(const FieldValue&)> setFunc;
	std::function<FieldValue()> getFunc;
};

struct Field
{
	FieldDefinition definition;
	FieldValue value;

	bool operator<(const Field& other) const
	{
		return this->definition.id < other.definition.id;
	}
};

struct FieldDisplay
{
	std::string name;
	std::string value;
};

using FieldDefinitions = std::vector<FieldDefinition>;
using FieldDisplays = std::vector<FieldDisplay>;

using SuccessOrErrorText = std::optional<std::string>;

} // namespace leo::component::config