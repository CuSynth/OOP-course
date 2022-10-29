#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <sstream>

#include "serializer.hpp"


template <typename K, typename V>
class network_storage final {
public:
	using key_t = K;
	using value_t = V;
	
	network_storage() = delete;
	network_storage(sf::IpAddress _IP, unsigned short _port) : IP(_IP), port(_port) {
		if (socket.connect(IP, port) != sf::TcpSocket::Done) {
			throw std::invalid_argument("Cant connect to server " + IP.toString() + ":" + std::to_string(port));
		}
	}

	void set(const key_t& key, const value_t& val);
	value_t get(const key_t& key);
	void disconnect() {
		socket.disconnect();
	}

	void remove(const key_t& key);


private:
	const sf::IpAddress IP;
	const unsigned short port;

	sf::TcpSocket socket;
	std::string send(const std::string& msg);
};

template <typename K, typename V>
void network_storage<K, V>::set(const network_storage<K, V>::key_t& key, const network_storage<K, V>::value_t& val) {
	std::ostringstream key_oss(std::ostringstream::out | std::ostringstream::binary);
	serialize(key, key_oss);
	std::string key_str(key_oss.str().c_str(), key_oss.str().size());

	std::ostringstream val_oss(std::ostringstream::out | std::ostringstream::binary);
	serialize(val, val_oss);
	std::string val_str = val_oss.str();

	std::string reply = send("set key=\"" + key_oss.str() + "\" value=\"" + val_oss.str() + "\"");

	if (reply.substr(0, 3) == "Err") {
		throw std::runtime_error("Error while writing " + reply);
	}
}

template <typename K, typename V>
network_storage<K, V>::value_t network_storage<K, V>::get(const network_storage<K, V>::key_t& key) {
	std::ostringstream key_oss(std::ostringstream::out | std::ostringstream::binary);
	serialize(key, key_oss);

	std::string reply = send(
		"get key=\"" + key_oss.str() + "\"");

	if (reply.empty()) {
		return value_t();
	}

	if (reply.substr(0, 3) == "Err") {
		throw std::runtime_error("Error while reading " + reply);
	}

	std::istringstream val_iss(reply, std::istringstream::in | std::istringstream::binary);
	val_iss >> std::noskipws;
	value_t val{};
	deserialize(val, val_iss);
	return val;
}


template <typename K, typename V>
void network_storage<K, V>::remove(const network_storage<K, V>::key_t& key) {
	std::ostringstream key_oss(std::ostringstream::out | std::ostringstream::binary);
	serialize(key, key_oss);

	std::string reply = send(
		"remove key=\"" + key_oss.str() + "\"");

	if (reply.substr(0, 3) == "Err") {
		throw std::runtime_error("Error while removing " + reply);
	}
}

template <typename K, typename V>
std::string  network_storage<K, V>::send(const std::string& msg) {
	sf::Packet packet;
	packet.append(msg.c_str(), msg.size());
	if (socket.send(packet) != sf::TcpSocket::Done) {
		throw std::runtime_error("Cant send packet");
	}

	packet.clear();
	if (socket.receive(packet) != sf::TcpSocket::Done) {
		throw std::runtime_error("Cant send packet");
	}

	return std::string(static_cast<const char*>(packet.getData()), packet.getDataSize());
}