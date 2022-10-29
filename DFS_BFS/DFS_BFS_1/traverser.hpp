#pragma once
#include "graph.hpp"
#include "strategy.hpp"


class traverser {
public:
	virtual void begin(vertex _src) = 0;
	virtual void end(bool strat_res) = 0;
	virtual bool visit_node(vertex _node) = 0;
	virtual bool visit_edge(edge_t _edge) = 0;

	void set_strategy(const std::shared_ptr<strategy>& _strat) { strat = _strat; }
	void set_graph(std::shared_ptr<graph>& _graph) { grph = _graph; }

	virtual void traverse(vertex _src) {
		strat->go(*grph, _src);
	};

	virtual ~traverser() {};

protected:
	vertex src;
	bool res = false;
	std::shared_ptr<strategy> strat{};
	std::shared_ptr<graph> grph{};

};

class node_traverser final : public traverser {
public:
	node_traverser() = delete;
	node_traverser(vertex _dest) {
		dest = _dest;
	}

	void begin(vertex _src) override { 
		reset(); 
		src = _src; 
	}
	void end(bool strat_res) override;
	bool visit_node(vertex _dest) override; 
	bool visit_edge(edge_t _edge) override;
	void reset();

	const std::vector<vertex>& get_path_nodes() const { return path_nodes; }
	const std::vector<edge_t>& get_path_edges() const { return path_edges; }
	const std::vector<vertex>& get_raw_nodes() const { return nodes; }
	const std::vector<edge_t>& get_raw_edges() const { return edges; }

	~node_traverser() = default;

private:
	std::vector<vertex> path_nodes{};
	std::vector<edge_t> path_edges{};
	std::vector<vertex> nodes{};
	std::vector<edge_t> edges{};

	vertex dest{};
};

class visitor final : public traverser {
public:
	visitor() {};

	void begin(vertex _src) override { 
		reset();
		src = _src; 
	};
	void end(bool strat_res) override;
	bool visit_node(vertex _node) override;
	bool visit_edge(edge_t _edge) override /*{ return false; }*/;

	void reset();
	const std::vector<vertex>& get_path_nodes() const { return nodes; }
	bool is_cyclic() { return cyclic; }

	~visitor() = default;

private:  
	std::vector<vertex> nodes{};
	std::vector<edge_t> edges{};

	bool cyclic = false;
};