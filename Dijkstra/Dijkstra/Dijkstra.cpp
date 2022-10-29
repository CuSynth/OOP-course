#include "Dijkstra.hpp"

void Dijkstra::calculate() {
	std::string minId;
	size_t min;
	size_t tmp;

	const table& tb = graph.getDat();

	do {
		min = UINT_MAX;
		minId.clear();

		for (auto &it : tb) {
			if (visited_vertexes[it.first] && (min_dists[it.first] < min)) {
				min = min_dists[it.first];
				minId = it.first;
			}
		}

		if (!minId.empty()) {
			for (auto &it : tb.at(minId)) {
				tmp = min + it.second;
				if (tmp < min_dists[it.first]) {
					min_dists[it.first] = tmp;
					min_ways[it.first] = minId;
				}
			}
			visited_vertexes[minId] = false;
		}
	} while (!minId.empty());	
}

std::string Dijkstra::getWay(std::string to) const{
	std::string newWay(to + "}");

	while (to != graph.getPivot()) {
		to = min_ways.at(to);
		newWay.insert(0, to + ", ");
	}
	newWay.insert(0, "{");

	return newWay;
}