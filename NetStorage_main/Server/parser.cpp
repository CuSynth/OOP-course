#include "parser.hpp"

std::shared_ptr<command> parser::parse(const std::string& cmd) {
	//if (std::regex_match(cmd, re)) {	// Без проверки, тк строка, приходящая от клиента может содержать \0 (== 0x00) внутри

	std::istringstream cmd_iss(cmd);
	std::string raw_cmd;

	cmd_iss >> raw_cmd;

	size_t arg_from = cmd.find("key=\"") + 5;
	size_t arg_to = cmd.find_last_of('"');
	std::string argument(cmd.substr(arg_from, arg_to - arg_from));

	return command::createFromStr(raw_cmd, argument, base);

	//}
	//else {
	//	throw std::invalid_argument("Unknown command: " + cmd);
	//}

}