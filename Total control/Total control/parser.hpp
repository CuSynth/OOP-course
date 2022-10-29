#ifndef __PARSER_H__
#define __PARSER_H__

#include "structs.hpp"

#include <stdexcept>
#include <sstream>

class DBase {

public:
	virtual void putMan(const std::string& place, const TimedMan& _man) = 0;

};

class Parser {

public:
	Parser(std::istream& _is) : is(_is) {}

	void parse(DBase& data) {
		std::string line;

		while (std::getline(is, line)) {
			std::istringstream iss(line);
			
			std::string place;
			TimedMan man;
			char c;

			if (line.empty())
				continue;

			if ((iss >> man.tBegin >> c >> man.tEnd >> man.UID) && (c == '-')) {
				if (man.tBegin > man.tEnd)
					throw std::invalid_argument("End is earlier than start at line: " + line);

				std::string pl;
				while (iss >> pl) {
					place += pl + " ";
				}

				if (place.empty())
					throw std::invalid_argument("Empty Place at line: " + line);

				data.putMan(place, man);

			}
			else
				throw std::invalid_argument("Wrong parametr at line: " + line);
		}
	}

private:
	std::istream& is;

};

#endif // !__PARSER_H__
