#include <iostream>
#include <sstream>
#include <limits.h>
#include <fstream>

#include "server.hpp"

//todo
// Иной подход к хранению, без переписывания всего файла при малом изменении
// Запись-чтение в один файл, заданный конструктором
// check connection
// 
// multiuser(+ stop cmd) - socketSelector
// multithread (каждый клиент в своем потоке. При чтении/записи испозуем блокипрвку)
//

constexpr unsigned short default_port = 1000;

int main(int argc, char** argv) {
	std::string fname;
	unsigned short port;
	int _port;

	if (argc == 1 || argc > 3) {
		std::cout << "Invalid argument! Usage:\n" <<
			argv[0] << " filename [port]" << std::endl;
		
		return -1;
	}
	else if (argc == 2) {
		port = default_port;
	}
	else {
		_port = std::stoi(argv[2]);

		if (_port > USHRT_MAX) {
			std::cout << "Port is too big. Use port less than 65 536" << std::endl;

			return -1;
		}

		port = static_cast<unsigned short>(_port);
	}
	
	fname = argv[1];

	std::cout << "Starting server on " << sf::IpAddress::getLocalAddress() << ":" << port <<
		"\nAnd writing to " << fname << std::endl;

	try {
		std::ifstream init(fname, std::ios::in | std::ios::binary);	//	Если делать без stringstream'а, то открытие ofs затирает данные
		std::string init_str;										//	Плохо. Надо переделать.

		std::string tmp;
		while (getline(init, tmp)){
			init_str.append(tmp);
		}

		init.close();
		std::istringstream is(init_str, std::ios::in | std::ios::binary);


		std::ofstream ofs(fname, std::ios::out | std::ios::binary);

		server srv(port, is, ofs);
		srv.process();
	}
	catch (const std::exception& ex) {
		std::cout << "Some error while running: " << ex.what() << std::endl;
	}

	std::cout << "Server stopped." << std::endl;

    return 0;
}