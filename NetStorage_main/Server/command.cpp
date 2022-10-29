#include "command.hpp"

std::shared_ptr<command> command::createFromStr(const std::string& cmd, const std::string& param, std::shared_ptr<storage> base) {

	if (cmd == "set") {
		return std::shared_ptr<command>(new set_command(param, base));
	}

	if (cmd == "get") {
		return std::shared_ptr<command>(new get_command(param, base));
	}

	if (cmd == "remove") {
		return std::shared_ptr<command>(new remove_command(param, base));
	}

	return nullptr;
}

set_command::set_command(const std::string& _key_value, std::shared_ptr<storage> _base) {
	size_t val_pos = _key_value.find("value=\"");
	key = _key_value.substr(0, val_pos - 2);
	value = _key_value.substr(val_pos + 7);
	base = _base;
}

void set_command::execute(std::string& external_buffer) {
	base->set(key, value);
}

get_command::get_command(const std::string& _key, std::shared_ptr<storage> _base) : key(_key) {
	base = _base;
}

void get_command::execute(std::string& external_buffer) {
	external_buffer = base->get(key);
};

remove_command::remove_command(const std::string& _key, std::shared_ptr<storage> _base) : key(_key) {
	base = _base;
}

void remove_command::execute(std::string& external_buffer) {
	base->remove(key);
}
