#include "strategy.hpp"
#include "traverser.hpp"
#include <deque>

bool strategy::traverser_visit_node(vertex _node) {
	auto strr = trr.lock();
	if (strr) {
		return strr->visit_node(_node);
	}

	return false;
}

bool strategy::traverser_visit_edge(edge_t _edge) {
	auto strr = trr.lock();
	if (strr) {
		return strr->visit_edge(_edge);
	}

	return false;
}

void DFS::go(graph& g, vertex from) {
	src = from;
	if (!g.node_exists(from)) {
		throw std::invalid_argument("There is no such node");
	}
	
	auto strr = trr.lock();
	strr->begin(from);
	strr->end(dfs(g));
}

bool DFS::dfs(graph& g) {
	if (traverser_visit_node(src)) {
		return true;
	}
	
	vertex cur = src;
	std::stack<edge_t> vs;
	grey.insert(cur);
	vs.push(std::make_pair(cur, -1));	// -1 - плохо

	while (!vs.empty()) {
		auto& edge = vs.top();
		cur = edge.first;
		const auto& ngbs = g.get_neighbours(cur);

		auto pos = ngbs.begin();
		if (edge.second != -1) {
			pos = ++std::find(ngbs.begin(), ngbs.end(), edge.second);
		}

		if(pos == ngbs.end()) {
			black.insert(cur);
			grey.erase(cur);
			vs.pop();
		} 
		else {
			edge.second = *pos;
			if (traverser_visit_edge(std::make_pair(edge.first, edge.second))) {
				return true;
			}

			if (!is_black(edge.second) && !is_grey(edge.second)) {
				vs.push(std::make_pair(edge.second, -1));
				grey.insert(edge.second);

				if (traverser_visit_node(edge.second)) {
					return true;
				}
			}
		}
	}

	return false;
}

void DFS::reset() {
	black.clear();
	grey.clear();
}

void BFS::go(graph& g, vertex from) {
	src = from;
	if (!g.node_exists(from)) {
		throw std::invalid_argument("There is no such node");
	}
	auto strr = trr.lock();
	strr->begin(from);
	strr->end(bfs(g));
}

bool BFS::bfs(graph& g) {
	if (traverser_visit_node(src)) {
		return true;
	}
	
	vertex cur = src;
	std::deque<vertex> vq;
	vq.push_back(cur);

	while (!vq.empty()) {
		cur = vq.front();
		vq.pop_front();
		black.insert(cur);

		for(vertex v: g.get_neighbours(cur)) {
			if (traverser_visit_edge(std::make_pair(cur, v))) {
				return true;
			}

			if (!is_black(v) &&
				(std::find(vq.begin(), vq.end(), v) == vq.end())) {
				vq.push_back(v);

				if (traverser_visit_node(v)) {
					return true;
				}
			}
		}
	}
	
	return false;
}

void BFS::reset() {
	black.clear();
	nodes.clear();
}
