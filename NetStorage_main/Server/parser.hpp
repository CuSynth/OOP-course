#pragma once
#include <regex>
#include <sstream>

#include "storage.hpp"
#include "command.hpp"

class parser final {
public:
	parser() = delete;
	parser(std::shared_ptr<storage> _base) : base(_base) {}

	std::shared_ptr<command> parse(const std::string& _cmd);

private:
	//std::regex re = std::regex(
	//"set[ \t]+key=\"[^\a\b\n\v\f\r]+\" value=\"[^\a\b\n\v\f\r]*\"[ \t]*"
	//"|[ \t]*get[ \t]+key=\"[^\a\b\n\v\f\r]+\"[ \t]*"
	//"|[ \t]*remove[ \t]+key=\"[^\a\b\n\v\f\r]+\"[ \t]*");

	std::shared_ptr<storage> base;

};
