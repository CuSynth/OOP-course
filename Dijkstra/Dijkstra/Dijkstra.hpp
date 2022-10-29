#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "graph.hpp"
#include "printer.hpp"

#include <limits>
#include <stdexcept>

class Dijkstra {
public:
	Dijkstra(const Graph& _graph) : graph(_graph) {
		if (!graph.Connective())
			throw std::invalid_argument("Grapf is not connective!\n");

		for (auto it : graph.getDat()) {
			min_dists[it.first] = UINT_MAX;
			visited_vertexes[it.first] = true;
		}
		min_dists[graph.getPivot()] = 0;
	}

	void calculate();

	std::string getWay(std::string to) const;
	const dist_map& getDists() const { return min_dists; }
	const std::string& getPivot() const { return graph.getPivot(); }

private:
	const Graph& graph;
	dist_map min_dists;
	ways_map min_ways;
	std::map<std::string, bool> visited_vertexes;
};


class DIter : public AlgoIter {
public:
	DIter(const Dijkstra& _obj) : obj(_obj), cur(_obj.getDists().begin()) {}
	
	virtual bool next() {
		cur++;
		return cur != obj.getDists().end();
	}

	virtual	std::string curWay() const { return obj.getWay((*cur).first); }
	virtual	const std::string& curCity() const { return (*cur).first; }
	virtual size_t curDist() const { return (*cur).second; }
	virtual const std::string& pivot() const { return obj.getPivot(); }

private:
	const Dijkstra& obj;
	dist_map::const_iterator cur;

};


#endif // !__Dijkstra_h__
