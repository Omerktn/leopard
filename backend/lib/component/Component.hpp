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
	using Publisher = core::io::Publisher;

public:
	Component(CompId compId, PublisherIndex publisherCount, core::io::ReceiverDefinition&& recvDef)
		: id{compId}
		, publishers(publisherCount, Publisher{"Unnamed publisher"})
		, receiverDef{std::move(recvDef)}
	{}

	virtual ~Component() = default;

	virtual void evaluate() = 0;
	virtual void initPublishers() = 0;

	void handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent)
	{
		if (idx >= receiverDef.slots.size())
		{
			throw std::runtime_error(
				"Input Index " + std::to_string(idx) +
				" is out of input-callbacks size: " + std::to_string(receiverDef.slots.size()));
		}

		auto& callback = receiverDef.slots.at(idx).callback;
		if (!callback)
		{
			throw std::runtime_error("Callback is not set for Input Index " + std::to_string(idx));
		}

		callback(anyEvent);
	}

	const core::io::ReceiverDefinition& getReceiverDefinition() const
	{
		return receiverDef;
	}

	Publisher& getPublisher(PublisherIndex idx)
	{
		return publishers.at(idx);
	}

private:
	const CompId id;
	std::vector<Publisher> publishers;
	const core::io::ReceiverDefinition receiverDef;
};

} // namespace leo
