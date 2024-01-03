#pragma once

#include <common/Types.hpp>

#include <core/io/Publisher.hpp>
#include <core/io/Receiver.hpp>

#include <logger/user/Logger.hpp>

#include <cstdint>
#include <type_traits>

namespace leo
{

class Component
{
public:
	Component(CompId compId,
			  logger::Logger&& compLogger,
			  core::io::PublisherSchema&& publisherSchema,
			  core::io::ReceiverSchema&& receiverSchema);

	virtual ~Component() = default;

	virtual void evaluate() = 0;

	CompId getId() const;

	void handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent);

	const core::io::ReceiverSchema& getReceiverSchema() const;
	core::io::Publisher& getPublisher(PublisherIndex idx);

	logger::Logger& getLogger();

protected:
	const CompId id;
	logger::Logger logger;

private:
	core::io::PublisherSchema publisherSchema;
	const core::io::ReceiverSchema receiverSchema;
};

} // namespace leo
