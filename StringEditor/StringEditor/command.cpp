#include "command.hpp"

std::shared_ptr<command> command::createFromStr(const std::string& cmd, const std::vector<std::string>& param, std::shared_ptr<document> txt) {

	if (cmd == "copy") {
		return std::shared_ptr<command>(new copy_cmd(param, txt));
	}

	if (cmd == "paste") {
		return std::shared_ptr<command>(new paste_cmd(param, txt));
	}

	if (cmd == "delete") {
		return std::shared_ptr<command>(new delete_cmd(param, txt));
	}

	if (cmd == "insert") {
		return std::shared_ptr<command>(new insert_cmd(param, txt));
	}

	if (cmd == "undo") {
		return std::shared_ptr<command>(new undo_cmd());
	}

	if (cmd == "redo") {
		return std::shared_ptr<command>(new redo_cmd());
	}

	return nullptr;
}

void copy_cmd::execute() {
	oldBuff = txt->getBuff();
	txt->setBuff(txt->substr(from, to));
}

void copy_cmd::unExec() {
	txt->setBuff(oldBuff);
}

void paste_cmd::execute() { 
	txt->insert(txt->getBuff(), pos);
}

void paste_cmd::unExec() { 
	txt->erase(pos, pos + txt->getBuff().size());
}

void insert_cmd::execute() { 
	txt->insert(data, pos);
}

void insert_cmd::unExec() { 
	txt->erase(pos, pos + data.size());
}

void delete_cmd::execute() {
	deleted = txt->substr(from, to);
	txt->erase(from, to);
}

void delete_cmd::unExec() { 
	txt->insert(deleted, from);
}