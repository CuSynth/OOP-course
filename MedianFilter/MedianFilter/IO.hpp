#ifndef __IO_H__
#define __IO_H__

#include <string>
#include <istream>

struct RawData {
	std::string IFName, OFName;
	size_t size, level;
} ;

RawData reader(std::istream& is);

#endif // !__IO_H__