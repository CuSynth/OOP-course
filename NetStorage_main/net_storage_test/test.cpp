#include "gtest/gtest.h"
#include <SFML/Network.hpp>
#include <sstream>
#include <thread>

#include "../Server/server.hpp"
#include "../Client/network_storage.hpp"

constexpr unsigned char port = 1000;


void start_server() {
	std::istringstream iss;
	std::ostringstream oss;

	server srv(port, iss, oss);
	srv.process();
}


TEST(net_storage, str_str) {
	//std::thread th(start_server);

	//network_storage<int, std::string> storage("localhost", port);

	//storage.set(1, "Vasya");
	//storage.set(2, "Petya");
	//storage.set(3, "Misha");

	//storage.remove(2);
	//storage.set(3, "nonMisha");

	//ASSERT_EQ(storage.get(1), "Vasya");
	//ASSERT_EQ(storage.get(2), "");
	//ASSERT_EQ(storage.get(3), "nonMisha");
	//storage.disconnect();

	//if (th.joinable())
	//	th.join();

}