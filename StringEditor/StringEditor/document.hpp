#pragma once
#include <string>
#include <iostream>

class document {
public:
	document() = default;
	document(std::istream& is);
	//document(document& another) : str(another.str) {}

	void setBuff(const std::string& _buff);
	std::string getBuff() const;

	void insert(const std::string& _str, size_t pos);

	void erase(size_t from, size_t to);

	std::string substr(size_t from, size_t to) const;

	void write(std::ostream& os) {
		os << str;
	}

private:
	std::string str {};
	std::string buff {};
};
