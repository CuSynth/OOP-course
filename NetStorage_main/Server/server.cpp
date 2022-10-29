#include "server.hpp"
#include <iostream>
#include <list>
#include <future>

#include "storage.hpp"
#include "parser.hpp"
#include "command.hpp"


server::server(unsigned short _port, std::istream& initStream, std::ostream& saveStream) : port(_port), os(saveStream) {
	base = std::make_shared<storage>();
	base->loadFrom(initStream);
}


void server::thProcess(sf::TcpSocket& client, std::ostream& saveStream) {
	parser prs(base);
	std::string buff;

	// if constexpr (run_info) {
	// 	std::cout << "accepted" << std::endl;
	// }


	sf::Packet packet;
	sf::TcpSocket::Status status;
	//client.setBlocking(false);

	while ((status = client.receive(packet)) != sf::TcpSocket::Disconnected && status != sf::TcpSocket::Error) {
		//	if(status == sf::TcpSocket::Done){
		std::string message(static_cast<const char*>(packet.getData()), packet.getDataSize());
		packet.clear();

		if constexpr (run_info) {
			std::cout << "Got from client " << client.getRemotePort() << ": " << message << std::endl;
		}

		try {
			locker.lock();
			prs.parse(message)->execute(buff);
			locker.unlock();
		}
		catch (const std::exception& ex) {
			message = "Err ";
			message += ex.what();
			packet.append(message.c_str(), message.size());

			if (client.send(packet) != sf::TcpSocket::Done) {
				throw std::runtime_error("Cant send error reply");
			}
			packet.clear();

			continue;
		}

		locker.lock();
		base->save(saveStream);
		locker.unlock();

		if (buff.empty() && message.substr(0, 3) != "get") {
			packet.append("Ack", 3);
			if (client.send(packet) != sf::TcpSocket::Done) {
				throw std::runtime_error("Cant send acknowledge");
			}
		}
		else {
			packet.append(buff.c_str(), buff.size());
			if (client.send(packet) != sf::TcpSocket::Done) {
				throw std::runtime_error("Cant send get reply");
			}

			buff.clear();
		}

		packet.clear();

		//	}
	}

	if (status == sf::TcpSocket::Error) {
		throw std::runtime_error("Server stopped by an error");
	}
}




void server::process() {
	// using TcpPtr = std::shared_ptr<sf::TcpSocket>;

	sf::TcpListener listener;
	sf::TcpSocket socket;
	sf::SocketSelector selector;
	std::list<sf::TcpSocket> clients;
	std::list<std::future<void>> threads;

	if (listener.listen(port) != sf::TcpListener::Done) {
		throw std::runtime_error("Cant start listening");
	}

	selector.add(listener);

	if constexpr (run_info) {
		std::cout << "lisening" << std::endl;
	}

	while (true) {
		if (selector.wait()) {
			if constexpr (run_info) {
				std::cout << "Waited" << std::endl;
			}

			if (selector.isReady(listener)) {

				sf::TcpSocket& cl = clients.emplace_back();
				if (listener.accept(cl) == sf::TcpSocket::Done) {
					threads.emplace_back(std::async(thProcess, std::ref(cl), std::ref(os)));

					if constexpr (run_info) {
						std::cout << "Connected client: " << cl.getRemotePort() << std::endl;
					}
				}
				else {
					throw std::runtime_error("Cant accept");
				}

			}
			else {
				throw std::runtime_error("Err while waiting!");
			}
		}
	}
}
