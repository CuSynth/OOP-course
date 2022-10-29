#include "parser.hpp"

void parser::parse() {
	for (std::string buff; std::getline(is, buff);) {
		try {
			std::vector<std::string> args;

			if (std::regex_match(buff, re)) {
				std::istringstream iss(buff);
				std::string tmp;

				while (iss >> tmp) {
					args.push_back(tmp);
				}
			}
			else {
				throw std::invalid_argument("Unknown editor command!\n");
			}

			commands.push_back(command::createFromStr(args[0], args, txt));
		}
		catch (const std::invalid_argument& exc) {
			std::string msg = "Error while parsing: ";
			msg += exc.what();
			msg += "Wrong string is: \"";
			msg += buff;
			msg += "\"\n";
			
			throw std::invalid_argument(msg);	
			// ��������, ����� �� ������������ ������, � ������ �������� � errorStream,
			// ������� ���������� parser'� ��� � ������������ (�� �������� �� StrEditor). ����� ��� ������������ ������� ������ ����� ������ � ����� ������
			// � ���������� ������.
		}
	}
}
