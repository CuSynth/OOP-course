#include <SFML/Network.hpp>
#include <iostream>	
#include <fstream>

#include "network_storage.hpp"

const sf::IpAddress default_ip("localhost");
constexpr unsigned short default_port = 1000;


int main(int argc, char** argv) {
	sf::IpAddress ip;
	unsigned short port;
	int _port;

	if (argc == 2) {
		std::cout << "Invalid argument! Usage:\n" <<
			argv[0] << " [ip port]" << std::endl;

		return -1;
	}
	else if (argc == 1) {
		std::cout << "Using default ip: " << default_ip << ":" << default_port << std::endl;
		ip = default_ip;
		port = default_port;
	}
	else {
		_port = std::stoi(argv[2]);

		if (_port > USHRT_MAX) {
			std::cout << "Port is too big. Use port less than 65 536" << std::endl;

			return -1;
		}

		port = static_cast<unsigned short>(_port);
	
		ip = sf::IpAddress(argv[1]);
	}


	try	{
		network_storage<int, std::string> storage(ip, port);

		storage.set(1, "first");
		storage.set(3, "second");
		storage.set(3, "third");
		storage.set(4, "fourth");

		storage.remove(4);

		std::string f = storage.get(1);
		std::string t = storage.get(3);
		std::string fo = storage.get(4);

		std::cout << f << t << fo << '\n';


		if (f == "first"
			&& t == "third"
			&& fo == "") 
		{
			std::cout << "Simple test successed!" << std::endl;
		}
		else {
			std::cout << "Simple test failed!" << std::endl;
		}
	}
	catch (const std::exception& ex) {
		std::cout << ex.what();
	}
	


	return 0;
}