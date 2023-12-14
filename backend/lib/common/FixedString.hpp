#pragma once

#include <array>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

namespace leo
{

/*
 * NULL-terminated stack-allocated fixed-size string
 */
template <std::size_t CAPACITY>
class FixedString
{
	static constexpr auto ARRAY_SIZE = CAPACITY + 1;

public:
	static constexpr auto MAX_SIZE = CAPACITY;

public:
	FixedString() = default;

	FixedString(const char* cStr)
	{
		const auto minSize = std::min(strlen(cStr), CAPACITY);
		std::memcpy(&data, cStr, minSize);

		readableSize = minSize;
		data[minSize] = '\0';
	}

	FixedString(const char* ptr, std::size_t size)
	{
		const auto minSize = std::min(size, CAPACITY);
		std::memcpy(&data, ptr, minSize);

		readableSize = minSize;
		data[minSize] = '\0';
	}

	template <std::size_t N>
	FixedString(const char (&cArrayString)[N])
		: FixedString(cArrayString, N)
	{}

	FixedString(const std::string_view sv)
		: FixedString(sv.data(), sv.size())
	{}

	FixedString(const FixedString<CAPACITY>& another)
		: data{another.data}
		, readableSize{another.readableSize}
	{}

	FixedString<CAPACITY>& operator=(const FixedString<CAPACITY>& another)
	{
		this->data = another.data;
		this->readableSize = another.readableSize;
		return *this;
	}

	FixedString<CAPACITY>& operator=(FixedString<CAPACITY>&& another)
	{
		this->data = std::move(another.data);
		this->readableSize = another.readableSize;
		return *this;
	}

	void assign(const unsigned char* cStr)
	{
		assign(reinterpret_cast<const char*>(cStr));
	}

	void assign(const char* cStr)
	{
		strncat(data.data(), cStr, MAX_SIZE);
		data[MAX_SIZE] = '\0';
		readableSize = strlen(data.data());
	}

	std::string_view toStringView() const
	{
		return {data.data(), readableSize};
	}

	std::string toString() const
	{
		return {data.data(), readableSize};
	}

	const char* getData() const noexcept
	{
		return data.data();
	}

	std::size_t getSize() const noexcept
	{
		return readableSize;
	}

	constexpr std::size_t getCapacity() const noexcept
	{
		return CAPACITY;
	}

private:
	std::array<char, ARRAY_SIZE> data;
	std::size_t readableSize = 0;
};
static_assert(std::is_standard_layout_v<FixedString<15>>);

template <std::size_t CAPACITY>
std::ostream& operator<<(std::ostream& os, const FixedString<CAPACITY>& fixedString)
{
	return os << fixedString.toStringView();
}

} // namespace leo