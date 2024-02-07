#include "Config.hpp"

#include <optional>

namespace leo::mdc
{

/*component::config::SuccessOrErrorText
Config::validate(ConfigFieldId fieldId, const component::config::FieldValue& valueVar)
{
	using namespace std::string_literals;
	using namespace component::config;

	switch (Field{fieldId.value()})
	{
	case Field::RANDOM_VAL: {
		const auto randomVal = std::get<decltype(Config::randomVal)>(valueVar);

		if (randomVal == 0)
		{
			return std::make_optional("RandomVal cannot be 0."s);
		}
		break;
	}
	case Field::PERIOD: {
		const auto period = std::get<decltype(Config::period)>(valueVar);

		if (period < MillisecondsFloat{0.0001})
		{
			return std::make_optional("Period cannot be near 0."s);
		}
		break;
	}
	default:
		return std::make_optional("Cannot handle fieldId: "s + std::to_string(fieldId.value()));
	}

	return std::nullopt; // No error
}
*/
} // namespace leo::mdc