#include "sTree.hpp"

#include <algorithm>
#include <stdexcept>

#include "structs.hpp"
#include "IO.hpp"

void MinSpanningTree::init(const vertex_vector& vertexes_) {
	vertexes = vertexes_;
	edges_init();
	DSU_init();
	isReady = false;
}

void MinSpanningTree::edges_init() {
	size_t size = vertexes.size();
	size_t n = size * (size - 1) / 2;
	edges.clear();
	edges.reserve(n);

	for (auto it = vertexes.begin(); it != vertexes.end(); ++it) {
		for (auto tIt = it + 1; tIt != vertexes.end(); ++tIt) {
			int dx = it->x - tIt->x;
			int dy = it->y - tIt->y;
			double dist = sqrt((dx * dx + dy * dy));
			edges.push_back(edge(it->id, tIt->id, dist));
		}
	}
}

void MinSpanningTree::DSU_init() {
	size_t size = vertexes.size();
	for (size_t i = 0; i < size; i++)
		DisjointSetUnion[vertexes[i].id] = vertexes[i].id;
}

size_t MinSpanningTree::DSU_root(size_t v) {
	if (DisjointSetUnion[v] == v)
		return v;
	return DisjointSetUnion[v] = DSU_root(DisjointSetUnion[v]);
}

bool MinSpanningTree::DSU_merge(size_t a, size_t b) {
	size_t ra = DSU_root(a);
	size_t rb = DSU_root(b);

	if (ra == rb)
		return false;

	DisjointSetUnion[ra] = rb;
	return true;
}

const mst& MinSpanningTree::Kruskal() {
	if (isReady)
		return tree;
	tree.clear();
	sort(edges.begin(), edges.end());
	for(edge e: edges)
		if (DSU_merge(e.a, e.b))
			tree.push_back({ e.a, e.b });
	isReady = true;

	return tree;
}