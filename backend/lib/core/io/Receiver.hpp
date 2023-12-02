#pragma once

//#include <component/Component.hpp>
#include <core/io/AnyEvent.hpp>

#include <functional>
#include <iostream>
#include <memory>

namespace leo::core::io
{

template <typename ComponentT, typename EventType>
class Receiver
{

public:
	explicit Receiver(InputIndex index, ComponentT& component)
		: component{component}
		, index{index}
	{}

	void dispatchToHandler3(const core::io::AnyEvent& anyEvent)
	{
		/*switch (idx)
		{
		case InputKind::BboSource::INDEX:
			dispatchToHandler<InputKind::BboSource>(anyEvent);
			break;
		case InputKind::HiSayer::INDEX:
			dispatchToHandler<InputKind::HiSayer>(anyEvent);
			break;
		default:
			throw std::runtime_error("Unexpected InputIndex: " + std::to_string(idx));
		}*/

		std::visit(leo::compose([](const ExpectedEvent& arg) { handle(InputType{}, arg); },
								[](auto) { std::cout << "Received something else.\n"; }),
				   anyEvent);
	}

	void dispatchToHandler(const core::io::AnyEvent& anyEvent)
	{
		using ExpectedEvent = typename InputType::EventType;

		const auto* eventPtr = std::get_if<ExpectedEvent>(&anyEvent);

		if (!eventPtr)
		{
			assert(!"Variant holds the wrong Event.");
			return;
		}

		handle(InputType{}, *eventPtr);
	}

	template <typename InputType>
	void dispatchToHandler2(const core::io::AnyEvent& anyEvent)
	{
		using ExpectedEvent = typename InputType::EventType;

		const auto* eventPtr = std::get_if<ExpectedEvent>(&anyEvent);

		if (!eventPtr)
		{
			assert(!"Variant holds the wrong Event.");
			return;
		}

		handle(InputType{}, *eventPtr);
		//std::visit(leo::compose([](const ExpectedEvent& arg) { handle(InputType{}, arg); },
		//						[](auto) { std::cout << "Received something else.\n"; }),
		//		   anyEvent);
	}

private:
	ComponentT& component;
	const InputIndex index;
};

} // namespace leo::core::io