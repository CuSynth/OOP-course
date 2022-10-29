#pragma once
#include <string>
#include <vector>

#include "document.hpp"
#include "command.hpp"
#include "parser.hpp"


class editor {
public:
	editor() = delete;
	editor(std::shared_ptr<document> _str) : str(_str) {}

	void apply(std::shared_ptr<command> cmd);
	void apply(const std::vector<std::shared_ptr<command>>& cmds);

	void undo() {
		if (commands.empty()) {
			return;
		}

		commands.back()->unExec();
		undos.emplace_back(std::move(commands.back()));
		commands.pop_back();
	}

	void redo() {
		if (undos.empty()) {
			return;
		}

		undos.back()->execute();
		commands.emplace_back(std::move(undos.back()));
		undos.pop_back();
	}

	void write(std::ostream& out) { str->write(out); }

private:
	std::shared_ptr<document> str;
	std::string buff;
	std::vector<std::shared_ptr<command>> commands;
	std::vector<std::shared_ptr<command>> undos;

	void exec(std::shared_ptr<command>& cmd) {
		cmd->execute();
		commands.push_back(cmd);
		undos.clear();
	}
};

