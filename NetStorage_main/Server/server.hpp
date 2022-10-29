#pragma once
#include <memory>
#include <mutex>
#include <SFML/Network.hpp>
#include "storage.hpp"

constexpr bool run_info = true;

class server {
public:
	server() = delete;
	server(unsigned short _port, std::istream& initStream, std::ostream& saveStream);

	void process();

private:
	unsigned short port;
	std::ostream& os;

	inline static std::shared_ptr<storage> base;
	inline static std::mutex locker;

	static void thProcess(sf::TcpSocket& client, std::ostream& saveStream);

};
