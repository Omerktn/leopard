#pragma once

namespace leo
{

template <auto F, typename T>
auto delegate(T& obj)
{
	return [&obj]() { return (obj.*F)(); };
}

} // namespace leo
