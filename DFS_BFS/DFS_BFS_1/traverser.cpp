#include "traverser.hpp"
#include "strategy.hpp"

bool node_traverser::visit_node(vertex _node) {
	nodes.push_back(_node);
	return _node == dest;
}

bool node_traverser::visit_edge(edge_t _edge) {
	edges.push_back(_edge);
	return false;
}

void node_traverser::reset() {
	path_nodes.clear();
	path_edges.clear();
	edges.clear();
	nodes.clear();
	strat->reset();
}

void node_traverser::end(bool strat_res) {
	res = strat_res;
	if (!res || edges.empty())
		return;
	
	auto right = --edges.end();
	path_edges.push_back(*right);
	path_nodes.push_back(right->second);

	while (right->first != src) {
		right = std::find_if(edges.begin(), edges.end(), 
			[right](auto& elem) {return elem.second == right->first; });
		path_edges.push_back(*right);
		path_nodes.push_back(right->second);
	}

	path_nodes.push_back(right->first);

	std::reverse(path_edges.begin(), path_edges.end());
	std::reverse(path_nodes.begin(), path_nodes.end());
}

void visitor::reset() {
	nodes.clear();
	strat->reset();
}

bool visitor::visit_node(vertex _node) {
	nodes.push_back(_node);
	return false;
}

bool visitor::visit_edge(edge_t _edge) {
	edges.push_back(_edge);
	return false;
}

void visitor::end(bool strat_res) {
	res = strat_res;
	std::vector<edge_t> fwd;

	for (auto& it : edges) {
		bool ex = std::find_if(fwd.begin(), fwd.end(), 
			[&it](auto& elem) {return it.first == elem.second && it.second == elem.first; }) == fwd.end();

		if (ex) {
			fwd.push_back(it);
		}
		else {
			continue;
		}
	}

	edges = fwd;

	auto preend = --edges.end();
	for (auto left = edges.begin(); left != preend; ++left) {
		for (auto right = left + 1; right != edges.end(); ++right) {
			if (left->second == right->second) {
				cyclic = true;
			}
		}
	}
}

