#pragma once

#include <type_traits>

namespace leo
{

template <typename... Types>
class TypeList
{};

template <typename List>
struct GetHead;

template <typename Head, typename... Tail>
struct GetHead<TypeList<Head, Tail...>>
{
	using Type = Head;
};

template <typename List>
using GetHeadT = GetHead<List>::Type;

// Pop Front
template <typename List>
struct PopFront;

template <typename Head, typename... Tail>
struct PopFront<TypeList<Head, Tail...>>
{
	using Type = TypeList<Tail...>;
};

template <typename List>
using PopFrontT = PopFront<List>::Type;

// Push Front
template <typename Typelist, typename Element>
struct PushFront;

template <typename... Typelist, typename Element>
struct PushFront<TypeList<Typelist...>, Element>
{
	using Type = TypeList<Element, Typelist...>;
};

template <typename List, typename Element>
using PushFrontT = PushFront<List, Element>::Type;

// Push Back
template <typename Typelist, typename Element>
struct PushBack;

template <typename... Typelist, typename Element>
struct PushBack<TypeList<Typelist...>, Element>
{
	using Type = TypeList<Typelist..., Element>;
};

template <typename List, typename Element>
using PushBackT = PushBack<List, Element>::Type;

// TODO move to test
namespace detail::test
{
using TestTypeList = TypeList<short, bool, int>;
static_assert(std::is_same<TypeList<bool, int>, PopFront<TestTypeList>::Type>());
static_assert(std::is_same<TypeList<int, short, bool, int>, PushFront<TestTypeList, int>::Type>());
static_assert(std::is_same<TypeList<short, bool, int, int>, PushBackT<TestTypeList, int>>());
} // namespace detail::test

} // namespace leo