#pragma once

#include "editor.hpp"

class StrEditor {
public:
	StrEditor(std::istream& dataStream, std::istream& commandStream, std::ostream& errorStream) :
		txt(std::make_shared<document>(dataStream)), prs(txt, commandStream), edt(txt), errStr(errorStream) {}

	void edit() {
		try {
			prs.parse();
			edt.apply(prs.getCommands());
		}
		catch (const std::exception& exc) {
			errStr << exc.what();
		}
	};

	void write(std::ostream& os) {
		try {
			txt->write(os);
		}
		catch (const std::exception& exc) {
			errStr << exc.what();
		}
	}

private:
	std::ostream& errStr;
	std::shared_ptr<document> txt;
	parser prs;
	editor edt;

};