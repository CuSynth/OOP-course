#include "parser.hpp"

#include <sstream>
#include <vector>

void Parser::parse(ParserClient& client) {
	std::string line, s;
	
	if (!(is >> s) || s != "S" || !(is >> s))
		throw std::invalid_argument("Wrong data in file!\n"); //WrongDataException();	
	client.putPivot(s);
	std::vector<std::string> strs(3);


	while (getline(is, line)) {
		strs.clear();

		std::istringstream iss(line);
		std::string tmp;

		while (getline(iss, tmp, del)) 
			strs.push_back(tmp);

		if (strs.size() == 0 || strs[0].size() == 0)
			continue;

		if(strs.size() != 3)
			throw std::invalid_argument("Wrong data in file!\n"); //WrongDataException();

		edge nEdge(strs[0], strs[1], (size_t)atoi(strs[2].data()));
		client.putEdge(nEdge);
	}
	client.finish();
}