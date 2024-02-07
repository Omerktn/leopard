#include "Component.hpp"

#include <core/Core.hpp>

namespace leo
{

Component::Component(core::Core& core,
					 CompId compId,
					 logger::Logger&& compLogger,
					 //component::config::FieldDefinitions&& configFields,
					 core::io::PublisherSchema&& publisherSchema,
					 core::io::ReceiverSchema&& receiverSchema)
	: core{core}
	, id{compId}
	, logger{std::move(compLogger)}
	, evalPreference{EvaluationPreference::AS_BUSY_AS_POSSIBLE}
	//, configDef{std::move(configFields)}
	, publisherSchema{std::move(publisherSchema)}
	, receiverSchema{std::move(receiverSchema)}
{}

EvaluationPreference Component::getEvaluationPreference() const
{
	return evalPreference;
}

void Component::handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent)
{
	if (idx.value() >= receiverSchema.slots.size())
	{
		throw std::runtime_error(
			"Input Index " + std::to_string(idx.value()) +
			" is out of input-callbacks size: " + std::to_string(receiverSchema.slots.size()));
	}

	auto& callback = receiverSchema.slots.at(idx.value()).callback;
	if (!callback)
	{
		throw std::runtime_error("Callback is not set for Input Index " +
								 std::to_string(idx.value()));
	}

	callback(anyEvent);
}

CompId Component::getId() const
{
	return id;
}

const core::io::ReceiverSchema& Component::getReceiverSchema() const
{
	return receiverSchema;
}

core::io::Publisher& Component::getPublisher(PublisherIndex idx)
{
	return publisherSchema.publishers.at(idx.value());
}

logger::Logger& Component::getLogger()
{
	return logger;
}

} // namespace leo