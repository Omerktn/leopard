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
using GetHeadT = typename GetHead<List>::Type;

// Pop Front
template <typename List>
struct PopFront;

template <typename Head, typename... Tail>
struct PopFront<TypeList<Head, Tail...>>
{
	using Type = TypeList<Tail...>;
};

template <typename List>
using PopFrontT = typename PopFront<List>::Type;

// Push Front
template <typename Typelist, typename Element>
struct PushFront;

template <typename... Typelist, typename Element>
struct PushFront<TypeList<Typelist...>, Element>
{
	using Type = TypeList<Element, Typelist...>;
};

template <typename List, typename Element>
using PushFrontT = typename PushFront<List, Element>::Type;

// Push Back
template <typename Typelist, typename Element>
struct PushBack;

template <typename... Typelist, typename Element>
struct PushBack<TypeList<Typelist...>, Element>
{
	using Type = TypeList<Typelist..., Element>;
};

template <typename List, typename Element>
using PushBackT = typename PushBack<List, Element>::Type;

// IndexOf functions

// Helper template to get the index of a type in the TypeList
template <typename T, typename TypeList>
struct IndexOf;

// Partial specialization for finding the index of a type in the TypeList
template <typename T, typename... Types>
struct IndexOf<T, TypeList<T, Types...>>
{
	static constexpr std::size_t value = 0;
};

template <typename T, typename U, typename... Types>
struct IndexOf<T, TypeList<U, Types...>>
{
	static constexpr std::size_t value = 1 + IndexOf<T, TypeList<Types...>>::value;
};

template <typename T, typename TypeList>
constexpr bool IndexOfV = IndexOf<T, TypeList>::value;

// Contains type

template <typename T, typename TypeListT>
struct ContainsType;

template <typename T>
struct ContainsType<T, TypeList<>>
{
	static constexpr bool value = false;
};

template <typename T, typename... Types>
struct ContainsType<T, TypeList<T, Types...>>
{
	static constexpr bool value = true;
};

template <typename T, typename U, typename... Types>
struct ContainsType<T, TypeList<U, Types...>>
{
	static constexpr bool value = ContainsType<T, TypeList<Types...>>::value;
};

template <typename T, typename TypeList>
constexpr bool ContainsTypeV = ContainsType<T, TypeList>::value;

// Is Empty

template <typename List>
struct IsEmpty;

template <>
struct IsEmpty<TypeList<>>
{
	static constexpr bool value = true;
};

template <typename... Types>
struct IsEmpty<TypeList<Types...>>
{
	static constexpr bool value = false;
};

// Helper alias for easier usage
template <typename List>
constexpr bool IsEmptyV = IsEmpty<List>::value;

// TODO move to test
namespace detail::test
{
using TestTypeList = TypeList<short, bool, int>;
static_assert(std::is_same<TypeList<bool, int>, PopFront<TestTypeList>::Type>());
static_assert(std::is_same<TypeList<int, short, bool, int>, PushFront<TestTypeList, int>::Type>());
static_assert(std::is_same<TypeList<short, bool, int, int>, PushBackT<TestTypeList, int>>());

static_assert(ContainsTypeV<bool, TypeList<short, bool, int>>);
static_assert(!ContainsTypeV<size_t, TypeList<short, bool, int>>);

static_assert(!IsEmptyV<TestTypeList>);
static_assert(IsEmptyV<TypeList<>>);
} // namespace detail::test

} // namespace leo