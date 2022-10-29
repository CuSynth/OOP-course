#include "printer.hpp"
#include <iostream>

void Printer::print(AlgoIter& client) {
	do
	{
		if (client.curCity() == client.pivot())
			continue;

		os << client.curCity() << " - " << client.curWay() << " - " << client.curDist() << std::endl;
	} while (client.next());
}