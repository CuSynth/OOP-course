#pragma once
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>

using vertex = size_t;
using edge_t = std::pair<size_t, size_t>;

struct covered_data {
	vertex v;
	std::vector<vertex> neighbours{};
};

class graph final {
public:
	using g_type = std::map<vertex, std::vector<vertex>>;

	graph() = default;
	graph(const std::vector<covered_data>& dat) {
		for (auto& elem : dat) {
			add_raw(elem);
		}

		recalculate_neighbours();
		_size = dat.size();
	}

	void add_node(const covered_data& dat) {
		add_raw(dat);
		recalculate_neighbours();
	}

	const std::vector<vertex>& get_neighbours(vertex v) const { return nodes.at(v); }
	bool node_exists(vertex v) const { return nodes.contains(v); }
	size_t size() { return _size; }

	const g_type& getGraph() const { return nodes; } // Testing..

	~graph() = default;

private:
	size_t _size = 0;
	g_type nodes{};

	void add_raw(const covered_data& dat) {
		vertex v = dat.v;
		const std::vector<vertex>& ngbrs = dat.neighbours;

		if (!nodes.count(v)) {
			nodes[v] = ngbrs;
		}
		else {
			nodes[v].insert(nodes[v].end(), ngbrs.begin(), ngbrs.end());
		}

		for (vertex nb : nodes[v]) {
			if (!nodes.count(nb)) {
				nodes[nb] = { v };
			}
			else {
				nodes[nb].push_back(v);
			}

		}
	}

	void recalculate_neighbours() {
		for (auto& elem : nodes) {
			sort(elem.second.begin(), elem.second.end(), std::less<vertex>());
			elem.second.erase(unique(elem.second.begin(), elem.second.end()), elem.second.end());
		}
	}

};