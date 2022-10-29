#include "gtest/gtest.h"
#include "../DFS_BFS_1/graph.hpp"
#include "../DFS_BFS_1/strategy.hpp"
#include "../DFS_BFS_1/traverser.hpp"

const std::vector<covered_data> dat{
					 {0, {}},
					 {1, {0}},
					 {2, {0}},
					 {3, {1}},
					 {4, {1}},
					 {5, {2}},
					 {6, {2}} };

const std::vector<covered_data> looped_dat{
				 {0, {}},
				 {1, {0}},
				 {2, {0}},
				 {3, {5, 1}},
				 {4, {6}},
				 {5, {2,1}},
				 {6, {2}} };


TEST(_graph, empty_ctr) {
	graph g;
}

TEST(_graph, single_ctr) {
	std::vector<covered_data> dat{ { 1, {} } };
	graph g(dat);
}

TEST(_graph, vect_ctr) {
	std::vector<covered_data> dat{ { 1, {2, 3} },
									{ 2, {3}} ,
									{ 3, {1}} };	// Repeted 1-3.


	graph g(dat);

}

TEST(_graph, data) {
	std::vector<covered_data> dat{ { 1, {2, 3} },
									{ 2, {3}},
									{ 3, {1}}, 	// Repeted 1-3.
									{ 1, {5}} }; // Added vertex 5 and edge 1->5

	graph g(dat);
	const graph::g_type& nodes = g.getGraph();

	ASSERT_EQ(nodes.size(), 4);

	std::vector<vertex> exp_1{ 2, 3, 5};
	std::vector<vertex> exp_2{ 1, 3 };
	std::vector<vertex> exp_3{ 1, 2 };
	std::vector<vertex> exp_5{ 1 };

	ASSERT_EQ(nodes.at(1), exp_1);
	ASSERT_EQ(nodes.at(2), exp_2);
	ASSERT_EQ(nodes.at(3), exp_3);
	ASSERT_EQ(nodes.at(5), exp_5);
}

TEST(node_trr, ctr_set) {
	auto tr = std::make_shared<node_traverser>(0);
	//tr->traverse(1);
}

TEST(visitor_trr, ctr_set) {
	auto tr = std::make_shared<visitor>();
	//tr->traverse(1);
}

TEST(dfs, ctr) {
	auto strat = std::make_shared<DFS>();
}

TEST(bfs, ctr) {
	auto strat = std::make_shared<BFS>();
}

TEST(node_trr, dfs) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<DFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

//	nodes = { 0, 1, 3, 4 };
//	ASSERT_EQ(tr->get_raw_nodes(), nodes);
//
//	edges = { {0, 1}, {1, 0}, {1, 3}, {3, 1}, {1, 4} };
//	ASSERT_EQ(tr->get_raw_edges(), edges);
}

TEST(node_trr, dfs_looped) {
	auto g = std::make_shared<graph>(looped_dat);
	auto strat = std::make_shared<DFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);
	
	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 3, 5, 2, 6, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_EQ(tr->get_raw_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 1}, {1, 3}, {3, 5}, {5, 2}, {2, 6}, {6, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

	//edges = { {0, 1}, {1, 0}, {1, 3}, {3, 1}, {3, 5}, {5, 1}, {5, 2}, {2, 0}, {2, 5}, {2, 6}, {6, 2}, {6, 4} };
	//ASSERT_EQ(tr->get_raw_edges(), edges);
}

TEST(node_trr, dfs_unreachable) {
	std::vector<covered_data> _dat{
					 {0, {}},
					 {1, {0}},
					 {2, {0, 1}},
					 {3, {4}} };

	auto g = std::make_shared<graph>(_dat);
	auto strat = std::make_shared<DFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);
	tr->traverse(0);

	std::vector<size_t> nodes{};
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{};
	ASSERT_EQ(tr->get_path_edges(), edges);
}

TEST(node_trr, bfs_unreachable) {
	std::vector<covered_data> _dat{
					 {0, {}},
					 {1, {0}},
					 {2, {0, 1}},
					 {3, {4}} };

	auto g = std::make_shared<graph>(_dat);

	auto strat = std::make_shared<BFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);
}

TEST(node_trr, bfs) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<BFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

}

TEST(node_trr, bfs_looped) {
	auto g = std::make_shared<graph>(looped_dat);
	auto strat = std::make_shared<BFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 2, 6, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 2}, {2, 6}, {6, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

	//nodes = { 0, 1, 2, 3, 5, 6, 4 };
	//ASSERT_EQ(tr->get_raw_nodes(), nodes);

	//edges = { {0, 1}, {0, 2}, {1, 3}, {1, 5}, {2, 6}, {0, 0}/*....*/};
	//ASSERT_EQ(tr->get_raw_edges(), edges);
}

TEST(node_trr, dfs_reusage) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<DFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);
	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

	tr->traverse(0);

	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_EQ(tr->get_path_edges(), edges);
}


TEST(node_trr, bfs_reusage) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<BFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

	tr->reset();
	tr->traverse(0);

	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_EQ(tr->get_path_edges(), edges);
}

TEST(node_trr, dfs_another_way) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<DFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

	tr->traverse(3);

	nodes = { 3, 1, 4};
	edges = { {3, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_EQ(tr->get_path_edges(), edges);

}


TEST(node_trr, bfs_another_way) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<BFS>();
	auto tr = std::make_shared<node_traverser>(4);

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);

	std::vector<std::pair<vertex, vertex>> edges{ {0, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_edges(), edges);

	tr->traverse(3);

	nodes = { 3, 1, 4 };
	edges = { {3, 1}, {1, 4} };
	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_EQ(tr->get_path_edges(), edges);

}



TEST(visitor_trr, dfs) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<DFS>();
	auto tr = std::make_shared<visitor>();

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 3, 4, 2, 5, 6 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_FALSE(tr->is_cyclic());
}

TEST(visitor_trr, dfs_looped) {
	auto g = std::make_shared<graph>(looped_dat);
	auto strat = std::make_shared<DFS>();
	auto tr = std::make_shared<visitor>();

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 3, 5, 2, 6, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_TRUE(tr->is_cyclic());
}

TEST(visitor_trr, bfs) {
	auto g = std::make_shared<graph>(dat);
	auto strat = std::make_shared<BFS>();
	auto tr = std::make_shared<visitor>();

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 2, 3, 4, 5, 6 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_FALSE(tr->is_cyclic());
}

TEST(visitor_trr, bfs_looped) {
	auto g = std::make_shared<graph>(looped_dat);
	auto strat = std::make_shared<BFS>();
	auto tr = std::make_shared<visitor>();

	strat->set_traverser(tr);
	tr->set_graph(g);
	tr->set_strategy(strat);

	tr->traverse(0);

	std::vector<size_t> nodes{ 0, 1, 2, 3, 5, 6, 4 };
	ASSERT_EQ(tr->get_path_nodes(), nodes);
	ASSERT_TRUE(tr->is_cyclic());
}
