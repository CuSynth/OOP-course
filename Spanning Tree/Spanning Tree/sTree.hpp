#ifndef __sTree_H__
#define __sTree_H__

#include <iostream>
#include <vector>
#include <map>

#include "structs.hpp"

class MinSpanningTree
{
public:
	MinSpanningTree() = default;
	MinSpanningTree(const vertex_vector& vertexes_) { init(vertexes_); };
	
	~MinSpanningTree() = default;
	
	void init(const vertex_vector& vertexes_);
	const mst& Kruskal();
private:

	bool isReady = false;
	
	std::vector<edge> edges;
	vertex_vector vertexes;
	std::map<size_t, size_t> DisjointSetUnion;
	mst tree;

	void edges_init();
	void DSU_init();
	size_t DSU_root(size_t v);
	bool DSU_merge(size_t a, size_t b);
};

#endif // !__sTree_H__
