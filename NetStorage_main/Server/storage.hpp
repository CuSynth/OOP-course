#pragma once
#include <map>
#include <string>
#include <iostream>


#include "serializer.hpp"


class storage final {
public:
	storage() = default;

	void loadFrom(std::istream& is);
	void save(std::ostream& os);

	void set(const std::string& key, const std::string& val);
	std::string& get(const std::string& key);
	void remove(const std::string& key);

private:
	std::map<std::string, std::string> base{};

};





