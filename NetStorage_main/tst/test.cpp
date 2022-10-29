#include "gtest/gtest.h"
#include <sstream>
#include <thread>
#include <vector>
#include <map>

#include "../Server/server.hpp"
#include "../Client/network_storage.hpp"

constexpr unsigned short port = 1000;


void start_server() {
	std::istringstream iss(std::ios::in | std::ios::binary);
	std::ostringstream oss(std::ios::out | std::ios::binary);

	server srv(port, iss, oss);
	srv.process();
}


TEST(a, b) {

}