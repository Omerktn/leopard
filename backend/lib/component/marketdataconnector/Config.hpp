#pragma once

#include <common/Chrono.hpp>
#include <common/Sugars.hpp>

#include <component/ConfigField.hpp>

#include <vector>

namespace leo::mdc
{

struct Config
{
	enum class Field : ConfigFieldId::UnderlyingType
	{
		RANDOM_VAL = 0,
		PERIOD,
	};

	component::config::FieldDefinitions getFieldsDefinition() const
	{
		using namespace component::config;
		return {FieldDefinition{
					.id = castToUnderlying(Field::RANDOM_VAL),
					.typeIndex = IndexOfV<decltype(randomVal), SupportedTypes>,
					.name = "RandomVal"
								/*.setFunc =
									[this](const FieldValue& valueVar) {
										this->randomVal = std::get<decltype(randomVal)>(valueVar);
									},*/
								.getFunc = [this]() { return FieldValue{this->randomVal}; }},
				FieldDefinition{
					.id = castToUnderlying(Field::PERIOD),
					.typeIndex = IndexOfV<decltype(period), SupportedTypes>,
					.name = "Period"
					/*.setFunc =
									[this](const FieldValue& valueVar) {
										this->period =
											std::get<decltype(period)>(valueVar); // serialize
									},
								.getFunc = [this]() { return FieldValue{this->period}; }*/
				}};
	}

	component::config::FieldDisplays getFieldValues() const
	{
		//
	}

	void setField(ConfigFieldId fieldId, const component::config::FieldValue& valueVar)
	{
		switch (Field{fieldId.value()})
		{
		case Field::RANDOM_VAL:
			const auto randomVal = std::get<decltype(Config::randomVal)>(valueVar);
			break;
		case Field::PERIOD:
			const auto period = MillisecondsFloat{std::get<decltype(Config::period)>(valueVar)};
			break;
		}
	}

	//static component::config::SuccessOrErrorText
	//validate(ConfigFieldId fieldId, const component::config::FieldValue& valueVar);

	static component::config::SuccessOrErrorText validate2(const Config& newConfig)
	{
		return std::nullopt;
	}

	uint32_t randomVal;
	MillisecondsFloat period;
};

} // namespace leo::mdc