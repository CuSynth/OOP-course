#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <string>
#include <map>

struct edge {
	std::string a, b;
	size_t dist;

	edge(std::string a, std::string b, size_t len) :a(a), b(b), dist(len) {};
};

using dist_map = std::map<std::string, size_t>;
using table = std::map<std::string, dist_map>;
using ways_map = std::map<std::string, std::string>;

#endif // !__STRUCTS_H__
