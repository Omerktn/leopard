#include "Types.hpp"

namespace leo
{

std::ostream& operator<<(std::ostream& os, const BboPrice& obj)
{
	os << "BBO( " << obj.bid.getAsDouble() << " / " << obj.ask.getAsDouble() << " )";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Bbo& obj)
{
	os << "BBO( " << obj.bid.getAsDouble() << " / " << obj.ask.getAsDouble() << " )-QTY( "
	   << obj.bidQty.getAsDouble() << " / " << obj.askQty.getAsDouble() << " )";
	return os;
}

} // namespace leo