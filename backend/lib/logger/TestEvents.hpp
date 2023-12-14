#pragma once

#include <common/FixedString.hpp>
#include <common/Types.hpp>

#include <type_traits>

namespace leo::log
{

class ReceivedArbitraryEvent1;
class ReceivedArbitraryEvent2;

struct ArbitraryEvent1
{
	using SERVER_SIDE_EVENT = ReceivedArbitraryEvent1;
	static constexpr auto EVENT_NAME = "ArbitraryEvent1";

	explicit ArbitraryEvent1(const std::string_view msgSv)
		: msg{msgSv}
	{}

	FixedString<63> msg;
} __attribute__((packed));
static_assert(std::is_standard_layout_v<ArbitraryEvent1>);

struct ArbitraryEvent2
{
	using SERVER_SIDE_EVENT = ReceivedArbitraryEvent2;
	static constexpr auto EVENT_NAME = "ArbitraryEvent2";

	explicit ArbitraryEvent2(const std::string_view msgSv, int val)
		: msg{msgSv}
		, val{val}
	{}

	FixedString<63> msg;
	int val;
} __attribute__((packed));
static_assert(std::is_standard_layout_v<ArbitraryEvent2>);

class ReceivedArbitraryEvent1
{};

class ReceivedArbitraryEvent2
{};

}; // namespace leo::log