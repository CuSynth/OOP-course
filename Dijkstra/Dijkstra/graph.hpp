#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "parser.hpp"
#include <vector>

class Graph : public ParserClient {
public:
	virtual void putEdge(const edge& _edge) { edges.push_back(_edge); }
	virtual void putPivot(const std::string& _city) { pivot_city = _city; }
	virtual void finish();

	bool Connective() const { return connective; }
	const table& getDat() const { return tb; }
	const std::string& getPivot() const { return pivot_city; }

private:

	bool connective = true;
	std::map<std::string, bool> vertexCon;
	void dfs(std::string st); // Обход в глубину

	std::vector<edge> edges;
	std::string pivot_city;
	table tb;
};


#endif // !__GRAPH_H__