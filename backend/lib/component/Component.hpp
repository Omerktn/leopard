#pragma once

#include <common/Types.hpp>

#include <core/io/AnyEvent.hpp>
#include <core/io/Publisher.hpp>

#include <cstdint>
#include <type_traits>

namespace leo
{

class Component
{
	using Publisher = core::io::Publisher<Component>;

	using InputCallback = std::function<void(const core::io::AnyEvent&)>;

protected:
	struct InputDefinition
	{
		struct Input
		{
			const std::type_info& eventTypeInfo;
			std::string name;
			std::string eventName;
			InputCallback callback;
		};

		using InputVector = std::vector<Input>;

		template <class... Inputs>
		InputDefinition(Inputs&&... inputs)
			: inputs{inputs...}
		{
			static_assert(std::conjunction_v<std::is_same<Input, Inputs>...>);
		}

		InputDefinition() = default;

		InputVector inputs{};
	};

public:
	Component(CompId compId, PublisherIndex publisherCount, const InputDefinition& inp)
		: id{compId}
		, publishers(publisherCount, Publisher{"Unnamed publisher"})
		, inputDef{inp}
	{}

	Component(CompId compId, PublisherIndex publisherCount)
		: id{compId}
		, publishers(publisherCount, Publisher{"Unnamed publisher2"})
		, inputDef{}
	{}

	virtual ~Component() = default;

	virtual void evaluate() = 0;
	virtual void initPublishers() = 0;
	virtual const InputDefinition& getInputDefinition()
	{
		return InputDefinition{};
	} // Make pure

	void handleAnyInput(InputIndex idx, const core::io::AnyEvent& anyEvent)
	{
		if (idx >= inputDef.inputs.size())
		{
			throw std::runtime_error(
				"Input Index " + std::to_string(idx) +
				" is out of input-callbacks size: " + std::to_string(inputDef.inputs.size()));
		}

		auto& callback = inputDef.inputs.at(idx).callback;
		if (!callback)
		{
			throw std::runtime_error("Callback is not set for Input Index " + std::to_string(idx));
		}

		callback(anyEvent);
	}

	Publisher& getPublisher(PublisherIndex idx)
	{
		return publishers.at(idx);
	}

private:
	const CompId id;
	std::vector<Publisher> publishers;
	//std::vector<InputCallback> inputCallbacks;
	InputDefinition inputDef;
};

} // namespace leo
