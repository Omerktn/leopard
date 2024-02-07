#pragma once

#include <component/ConfigField.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace leo::component::config
{

class ConfigurationManager
{
public:
private:
public:
	explicit ConfigurationManager();

	ConfigurationManager(ConfigurationManager&&) = delete;
	ConfigurationManager(const ConfigurationManager&) = delete;
	ConfigurationManager& operator=(ConfigurationManager&&) = delete;
	ConfigurationManager& operator=(const ConfigurationManager&) = delete;
	~ConfigurationManager();

	void updateField(ConfigFieldId fieldId, const FieldValue& fieldVar);

private:
	//

	Field* getField(ConfigFieldId fieldId);

private:
	std::vector<Field> fields;
};

} // namespace leo::component::config
