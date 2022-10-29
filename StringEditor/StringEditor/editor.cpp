#include "editor.hpp"

void editor::apply(std::shared_ptr<command> cmd) {
	if (!cmd.get()) {
		return;
	}

	if (undo_cmd* undoP = dynamic_cast<undo_cmd*>(cmd.get())) {
		undo();
		return;
	}

	if (redo_cmd* redoP = dynamic_cast<redo_cmd*>(cmd.get())) {
		redo();
		return;
	}

	exec(cmd);
}

void editor::apply(const std::vector<std::shared_ptr<command>>& cmds) {
	for (auto& cmd : cmds) {
		apply(cmd);
	}
}
