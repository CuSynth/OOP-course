#pragma once
#include <regex>
#include <sstream>

#include "document.hpp"
#include "command.hpp"

class parser final {
public:
	parser() = delete;
	parser(std::shared_ptr<document> _txt, std::istream& _is) : is(_is), txt(_txt){}

	void parse();
	std::vector<std::shared_ptr<command>>& getCommands() { return commands; }

private:
	std::regex re = std::regex(
		"[ \t]*copy[ \t]+[0-9]+,[ \t]+[0-9]+[ \t]*|[ \t]*paste[ \t]+[0-9]+[ \t]*|[ \t]*insert[ \t]+\"[^\a\b\n\v\f\r]+\",[ \t]+[0-9]+[ \t]*|[ \t]*delete[ \t]+[0-9]+,[ \t]+[0-9]+[ \t]*|[ \t]*undo[ \t]*|[ \t]*redo[ \t]*");

	std::istream& is;
	std::shared_ptr<document> txt;
	std::vector<std::shared_ptr<command>> commands;
};
