#pragma once

#include <common/Types.hpp>

#include <core/io/Publisher.hpp>
#include <core/io/Receiver.hpp>

#include <cstdint>
#include <type_traits>

namespace leo
{

class Component
{
public:
	Component(CompId compId,
			  core::io::PublisherSchema&& publisherSchema,
			  core::io::ReceiverSchema&& receiverSchema);

	virtual ~Component() = default;

	virtual void evaluate() = 0;

	void handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent);

	const core::io::ReceiverSchema& getReceiverSchema() const;
	core::io::Publisher& getPublisher(PublisherIndex idx);

private:
	const CompId id;
	core::io::PublisherSchema publisherSchema;
	const core::io::ReceiverSchema receiverSchema;
};

} // namespace leo
