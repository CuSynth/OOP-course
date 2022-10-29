#ifndef __PSRSER_H__
#define __PSRSER_H__

#include "structs.hpp"

#define del ' '

class ParserClient {
public:
	virtual void putEdge(const edge& _edge) {}
	virtual void putPivot(const std::string& _city) {}
	virtual void finish() {}
};

class Parser {
public:
	Parser(std::istream& _is) : is(_is) {}
	void parse(ParserClient& client);
	//class WrongDataException : public std::exception {}
private:
	std::istream &is;
};


#endif // !__PSRSER_H__