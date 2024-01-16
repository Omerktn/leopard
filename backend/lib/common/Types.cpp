#include "Types.hpp"

namespace leo
{

std::ostream& operator<<(std::ostream& os, const BboPrice& obj)
{
	os << "BBO( " << obj.bid.value().getAsDouble() << " / " << obj.ask.value().getAsDouble()
	   << " )";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Bbo& obj)
{
	os << "BBO( " << obj.bid.value().getAsDouble() << " / " << obj.ask.value().getAsDouble()
	   << " )-QTY( " << obj.bidQty.value().getAsDouble() << " / "
	   << obj.askQty.value().getAsDouble() << " )";
	return os;
}

} // namespace leo