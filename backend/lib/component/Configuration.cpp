#include "Configuration.hpp"

namespace leo::component::config
{

ConfigurationManager::ConfigurationManager()
	: fields{}
{
	//
}

ConfigurationManager::~ConfigurationManager() {}

void ConfigurationManager::updateField(ConfigFieldId fieldId, const FieldValue& fieldVar) {}

Field* ConfigurationManager::getField(ConfigFieldId fieldId)
{
	auto tempDefinition = FieldDefinition{.id = fieldId};
	auto it = std::lower_bound(
		fields.begin(), fields.end(), Field{.definition = std::move(tempDefinition)});

	if (it == fields.end())
	{
		return nullptr;
	}

	return it->definition.id == fieldId ? &(*it) : nullptr;
}

} // namespace leo::component::config