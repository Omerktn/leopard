#include "Component.hpp"

namespace leo
{

Component::Component(CompId compId,
					 logger::Logger&& compLogger,
					 core::io::PublisherSchema&& publisherSchema,
					 core::io::ReceiverSchema&& receiverSchema)
	: id{compId}
	, logger{std::move(compLogger)}
	, publisherSchema{std::move(publisherSchema)}
	, receiverSchema{std::move(receiverSchema)}
{}

void Component::handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent)
{
	if (idx >= receiverSchema.slots.size())
	{
		throw std::runtime_error(
			"Input Index " + std::to_string(idx) +
			" is out of input-callbacks size: " + std::to_string(receiverSchema.slots.size()));
	}

	auto& callback = receiverSchema.slots.at(idx).callback;
	if (!callback)
	{
		throw std::runtime_error("Callback is not set for Input Index " + std::to_string(idx));
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
	return publisherSchema.publishers.at(idx);
}

} // namespace leo