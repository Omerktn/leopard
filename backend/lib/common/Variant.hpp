#pragma once

#include <type_traits>
#include <variant>

namespace leo
{

namespace detail
{

template <typename... lambda_ts>
struct composer_t;

template <typename lambda_t>
struct composer_t<lambda_t> : lambda_t
{
	composer_t(const lambda_t& lambda)
		: lambda_t{lambda}
	{}
	composer_t(lambda_t&& lambda)
		: lambda_t{std::move(lambda)}
	{}

	using lambda_t::operator();
};

template <typename lambda_t, typename... more_lambda_ts>
struct composer_t<lambda_t, more_lambda_ts...> : lambda_t, composer_t<more_lambda_ts...>
{
	using super_t = composer_t<more_lambda_ts...>;

	template <typename... lambda_ts>
	composer_t(const lambda_t& lambda, lambda_ts&&... more_lambdas)
		: lambda_t{lambda}
		, super_t{std::forward<lambda_ts>(more_lambdas)...}
	{}
	template <typename... lambda_ts>
	composer_t(lambda_t&& lambda, lambda_ts&&... more_lambdas)
		: lambda_t{std::move(lambda)}
		, super_t{std::forward<lambda_ts>(more_lambdas)...}
	{}

	using lambda_t::operator();
	using super_t::operator();
};

template <typename>
struct tag
{};

} // namespace detail

template <typename... lambda_ts>
auto compose(lambda_ts&&... lambdas)
{
	return detail::composer_t<std::decay_t<lambda_ts>...>{std::forward<lambda_ts>(lambdas)...};
}

// -----------------------------------------------------------------------------
// Stole from https://en.cppreference.com/w/cpp/utility/variant/visit

template <class... Ts>
struct Overloaded : Ts...
{
	using Ts::operator()...;
};

template <typename VariantType, typename T, std::size_t index = 0>
constexpr std::size_t getVariantIndex()
{
	static_assert(std::variant_size_v<VariantType> > index, "Type not found in variant");
	if constexpr (index == std::variant_size_v<VariantType>)
	{
		return index;
	}
	else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>,
									  std::decay_t<T>>)
	{
		return index;
	}
	else
	{
		return getVariantIndex<VariantType, std::decay_t<T>, index + 1>();
	}
}

} // namespace leo