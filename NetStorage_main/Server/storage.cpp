#include "storage.hpp"

void storage::loadFrom(std::istream& is) {
	if (is.eof() || is.rdbuf()->in_avail() == 0) {
		return;
	}

	is >> std::noskipws;
	deserialize(base, is);
}

void storage::save(std::ostream& os) {
	os.seekp(0);
	serialize(base, os);
}

void storage::set(const std::string& key, const std::string& val) {
	base[key] = val;
}

std::string& storage::get(const std::string& key) {
	return base[key];
}

void storage::remove(const std::string& key) {
	base.erase(key);
}
