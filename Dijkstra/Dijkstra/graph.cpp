#include "graph.hpp"

void Graph::finish() {
	for (auto it : edges) {
		tb[it.a][it.b] = it.dist;
		tb[it.b][it.a] = it.dist;
		vertexCon[it.a] = false;
		vertexCon[it.b] = false;
	}

	dfs(pivot_city);
	for (auto it : tb)
		connective = connective && vertexCon[it.first];
}

void Graph::dfs(std::string st) {
	vertexCon[st] = true;
	for (auto it : tb[st])
		if (!vertexCon[it.first])
			dfs(it.first);
}