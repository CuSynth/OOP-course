#ifndef __STRUCTS_H__
#define __STRUCTS_H__

struct edge {
	size_t a, b;
	double len;

	edge(size_t a = 0, size_t b = 0, double len = 0) :a(a), b(b), len(len) {};

	bool operator<(const edge& rhs) { return len < rhs.len; };
};


struct vertex {
	size_t id;
	int x, y;

	vertex(size_t id = 0, int x = 0, int y = 0) : id(id), x(x), y(y) {};
};

using mst = std::vector<std::pair<size_t, size_t>>;
using vertex_vector = std::vector<vertex>;


#endif // !__STRUCTS_H__