#include "document.hpp"

document::document(std::istream& is) {
	//if (!is)
	//	throw std::invalid_argument("Invalid input stream\n");
	
	std::getline(is, str);
}

void document::setBuff(const std::string& _buff) {
	buff = _buff;
}

std::string document::getBuff() const {
	return buff;
}

void document::insert(const std::string& _str, size_t pos) {
	if (pos > str.size()) {
		throw std::out_of_range("Str is out of range\n");
	}

	str.insert(pos, _str);
}

std::string document::substr(size_t from, size_t to) const {
	if (from >= str.size()) {
		throw std::out_of_range("Str is out of range\n");
	}

	return	str.substr(from, to - from);
}

void document::erase(size_t from, size_t to) {
	if (from > str.size()) {
		throw std::out_of_range("Str is out of range\n");
	}

	str.erase(from, to - from);
}
