#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "structs.hpp"

class AlgoIter {
public:
	virtual bool next() = 0;
	virtual std::string curWay() const = 0;
	virtual const std::string& curCity() const = 0;
	virtual size_t curDist() const = 0;
	virtual const std::string& pivot() const = 0;

};

class Printer {
public:
	Printer(std::ostream& _os) : os(_os) {};
	void print(AlgoIter& iter);

private:
	std::ostream& os;

};

#endif // !__PRINTER_H__
