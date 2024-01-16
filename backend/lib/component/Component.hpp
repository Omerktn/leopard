#pragma once

#include <component/Evaluation.hpp>

#include <common/Types.hpp>

#include <core/io/Publisher.hpp>
#include <core/io/Receiver.hpp>

#include <logger/user/Logger.hpp>

#include <cstdint>
#include <type_traits>

namespace leo
{

namespace core
{
class Core;
}

class Component
{
public:
	Component(core::Core& core,
			  CompId compId,
			  logger::Logger&& compLogger,
			  core::io::PublisherSchema&& publisherSchema,
			  core::io::ReceiverSchema&& receiverSchema);

	virtual ~Component() = default;

	virtual void evaluate(const EvaluationContext&) = 0;

	EvaluationPreference getEvaluationPreference() const;

	CompId getId() const;

	void handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent);

	const core::io::ReceiverSchema& getReceiverSchema() const;
	core::io::Publisher& getPublisher(PublisherIndex idx);

	logger::Logger& getLogger();

public:
	Nanoseconds lastEvaluationTime{0};

protected:
	core::Core& core;
	const CompId id;
	logger::Logger logger;
	EvaluationPreference evalPreference;

private:
	core::io::PublisherSchema publisherSchema;
	const core::io::ReceiverSchema receiverSchema;
};

} // namespace leo
