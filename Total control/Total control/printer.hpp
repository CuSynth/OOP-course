#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "calculator.hpp"

#include <sstream>

class Printer {
public:
	Printer(std::ostream& _os) : os(_os) {}
	
	void print(const std::vector<Crowd>& crowds) {
		for (auto it : crowds) {
			os << "Place: " << it.place << std::endl
				<< "  Begining: " << it.tBegin << "; Ending: " << it.tEnd << std::endl
				<< "  Max count: " << it.maxCount << "; AVG count: " << it.avgCount << std::endl
				<< "  People:" << std::endl;
			for (auto sit : it.pList)
				os << "    " << sit << std::endl;
		}
	}

private:
	std::ostream& os;
};


#endif // !__PRINTER_H__