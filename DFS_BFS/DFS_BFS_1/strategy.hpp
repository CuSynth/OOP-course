#pragma once
#include <stack>
#include <set>
#include "graph.hpp"

class traverser;

class strategy {
public:
	virtual void go(graph& _graph, vertex from) = 0;

	void set_traverser(std::shared_ptr<traverser> _traverser) { trr = _traverser; }
	bool traverser_visit_node(vertex _node);
	bool traverser_visit_edge(edge_t _edge);
	virtual void reset() = 0;

	virtual ~strategy() {};

protected:
	vertex src;
	std::set<vertex> black;

	bool is_black(vertex v) const {
		return std::find(black.begin(), black.end(), v) != black.end();
	}
	
	std::weak_ptr<traverser> trr{};
};

class DFS final : public strategy {
public:
	void go(graph& g, vertex from) override;
	void reset() override;

	~DFS() = default;

private:
	bool dfs(graph& g);

	std::set<vertex> grey;
	bool is_grey(vertex v) const {
		return std::find(grey.begin(), grey.end(), v) != grey.end();
	}
};

class BFS final : public strategy {
public:
	void go(graph& g, vertex from) override;
	void reset() override;

	~BFS() = default;

private:
	bool bfs(graph& g);

	std::deque<vertex> nodes{};
};
