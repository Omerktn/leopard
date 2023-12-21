#pragma once

#include <common/FixedString.hpp>
#include <common/Types.hpp>
#include <logger/user/LogFields.hpp>

#include <type_traits>
#include <variant>

namespace leo::log
{

struct ReceivedArbitraryEvent1;
struct ReceivedArbitraryEvent2;

struct ArbitraryEvent1
{
	using SERVER_SIDE_EVENT = ReceivedArbitraryEvent1;

	explicit ArbitraryEvent1(const std::string_view msgSv)
		: msg{msgSv}
	{}

	FixedString<63> msg;
} __attribute__((packed));
static_assert(std::is_standard_layout_v<ArbitraryEvent1>);
static_assert(sizeof(ArbitraryEvent1) == 64 + 8);

struct ArbitraryEvent2
{
	using SERVER_SIDE_EVENT = ReceivedArbitraryEvent2;

	explicit ArbitraryEvent2(const std::string_view msgSv, int val)
		: msg{msgSv}
		, val{val}
	{}

	FixedString<63> msg;
	int val;
} __attribute__((packed));
static_assert(std::is_standard_layout_v<ArbitraryEvent2>);

struct ReceivedArbitraryEvent1
{
	static constexpr auto EVENT_NAME = "ArbitraryEvent1";

	explicit ReceivedArbitraryEvent1(const ArbitraryEvent1& userEvent)
		: r_msg_name{"Message"}
		, r_msg{userEvent.msg.toStringView()}
	{}

	[[nodiscard]] logger::Fields getFields() const
	{
		using namespace leo::logger;
		return Fields{Field{r_msg_name, FieldVariant{std::ref(r_msg)}}};
	}

	logger::FieldName r_msg_name;
	std::string r_msg;
};

struct ReceivedArbitraryEvent2
{
	static constexpr auto EVENT_NAME = "ArbitraryEvent2";

	explicit ReceivedArbitraryEvent2(const ArbitraryEvent2& userEvent)
		: r_msg_name{"Message"}
		, r_msg{userEvent.msg.toStringView()}
		, r_val_name{"TheValue"}
		, r_val{userEvent.val}
	{}

	[[nodiscard]] logger::Fields getFields() const
	{
		using namespace leo::logger;
		return Fields{Field{r_msg_name, FieldVariant{std::ref(r_msg)}},
					  Field{r_val_name, FieldVariant{r_val}}};
	}

	logger::FieldName r_msg_name;
	std::string r_msg;

	logger::FieldName r_val_name;
	decltype(ArbitraryEvent2::val) r_val;
};

}; // namespace leo::log