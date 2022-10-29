#include "IO.hpp"

#include <stdexcept>

RawData reader(std::istream& is) {
	RawData data;

	if (!(is >> data.IFName >> data.size >> data.level >> data.OFName))
		throw std::invalid_argument( "Wrong element in file!\n" );

	return data;
}