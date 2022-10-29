#include "gtest/gtest.h"
#include "../Dijkstra/Dijkstra.hpp"

using namespace std;

string simple_input = "S Moscow\n"
					  "Moscow Novosibirsk 7";

string simple_pivot = "Moscow";

edge simple_edge("Moscow", "Novosibirsk", 7);
edge another_edge("nMoscow", "nNovosibirsk", 7);

string full_input = "S Moscow\n"
					"Moscow Novosibirsk 7\n"
					"Moscow Toronto 9\n"
					"Moscow Krasnoyarsk 14\n"
					"Novosibirsk Toronto 10\n"
					"Novosibirsk Omsk 15\n"
					"Omsk Toronto 11\n"
					"Toronto Krasnoyarsk 2\n"
					"Krasnoyarsk Kiev 9\n"
					"Kiev Omsk 6";

string incorrect_start_city_input = "s notMoscow\n"
									"Moscow Novosibirsk 7";

string incorrect_input = "S Moscow\n"
						 "this is notMoscow and not Novosibirsk and not 7";


string simple_output = "Novosibirsk - {Moscow, Novosibirsk} - 7\n";

string full_output = "Kiev - {Moscow, Toronto, Krasnoyarsk, Kiev} - 20\n"
 					 "Krasnoyarsk - {Moscow, Toronto, Krasnoyarsk} - 11\n"
 					 "Novosibirsk - {Moscow, Novosibirsk} - 7\n"
 					 "Omsk - {Moscow, Toronto, Omsk} - 20\n"
 					 "Toronto - {Moscow, Toronto} - 9\n";

string nonConnective_input = "S Moscow\n"
 							 "Moscow Novosibirsk 7\n"
 							 "nMoscow nNovosibirsk 9\n";

TEST(Ctrs, EmptyArg) {
	Graph graph;
}

TEST(Ctrs, notEmptyArg) {
	Graph graph;
	Dijkstra DAlg(graph);
	DIter iter(DAlg);

	Parser prs(cin);
	Printer prt(cout);
}

TEST(_Graph, TableCreation) {
	Graph graph;
	graph.putEdge(simple_edge);
	graph.putPivot(simple_pivot);
	graph.finish();

	table tb;
	tb[simple_edge.a][simple_edge.b] = 7;
	tb[simple_edge.b][simple_edge.a] = 7;

	ASSERT_EQ(graph.getDat(), tb);
}

TEST(_Graph, PivotInsertion) {
	Graph graph;
	graph.putPivot(simple_pivot);

	ASSERT_EQ(graph.getPivot(), simple_pivot);
}

TEST(_Graph, Connectivity) {
	Graph graph;
	graph.putPivot(simple_pivot);
	graph.putEdge(simple_edge);
	graph.putEdge(another_edge);
	graph.finish();

	ASSERT_FALSE(graph.Connective());
}

TEST(_Parser, GoodInp) {
	istringstream iss(simple_input);
	
	Parser prs(iss);
	Graph graph;

	prs.parse(graph);

	table tb;
	tb[simple_edge.a][simple_edge.b] = 7;
	tb[simple_edge.b][simple_edge.a] = 7;
	
	ASSERT_EQ(graph.getDat(), tb);
	ASSERT_EQ(graph.getPivot(), simple_pivot);
}

TEST(_Parser, BadStartCityInp) {
	istringstream iss(incorrect_start_city_input);
	
	Parser prs(iss);
	Graph graph;

	ASSERT_THROW(prs.parse(graph), invalid_argument);
}

TEST(_Parser, BadInp) {
	istringstream iss(incorrect_input);

	Parser prs(iss);
	Graph graph;

	ASSERT_THROW(prs.parse(graph), invalid_argument);
}

TEST(_Parser, nonConInp) {
	istringstream iss(nonConnective_input);

	Parser prs(iss);
	Graph graph;
	prs.parse(graph);

	ASSERT_FALSE(graph.Connective());
	ASSERT_THROW(Dijkstra DAlg(graph), invalid_argument);
}

TEST(_Dijkstra, Simple) {
	istringstream iss(simple_input);
	Graph graph;
	Parser prs(iss);
	prs.parse(graph);

	Dijkstra DAlg(graph);
	DAlg.calculate();
	
	dist_map simple_dmap;
	simple_dmap["Moscow"] = 0;
	simple_dmap["Novosibirsk"] = 7;
	ASSERT_EQ(simple_dmap, DAlg.getDists());
	
	ASSERT_EQ("{Moscow, Novosibirsk}", DAlg.getWay("Novosibirsk"));
	ASSERT_EQ("{Moscow}", DAlg.getWay("Moscow"));
}

TEST(_AlgoIter, Simple) {
	istringstream iss(simple_input);

	Graph graph;
	Parser prs(iss);
	prs.parse(graph);

	Dijkstra DAlg(graph);
	DAlg.calculate();
	DIter dit(DAlg);

	ASSERT_EQ(dit.curCity(), "Moscow");
	ASSERT_EQ(dit.curDist(), 0);
	ASSERT_EQ(dit.curWay(), "{Moscow}");
	ASSERT_EQ(dit.pivot(), simple_pivot);
	
	ASSERT_TRUE(dit.next());

	ASSERT_EQ(dit.curCity(), "Novosibirsk");
	ASSERT_EQ(dit.curDist(), 7);
	ASSERT_EQ(dit.curWay(), "{Moscow, Novosibirsk}");
	ASSERT_EQ(dit.pivot(), simple_pivot);

	ASSERT_FALSE(dit.next());

}

TEST(_Printer, Simple) {
	istringstream iss(simple_input);

	Graph graph;
	Parser prs(iss);
	prs.parse(graph);

	Dijkstra DAlg(graph);
	DAlg.calculate();

	ostringstream oss;
	Printer prt(oss);

	DIter dit(DAlg);

	prt.print(dit);

	ASSERT_EQ(oss.str(), simple_output);
}

TEST(_Printer, Full) {
	istringstream iss(full_input);

	Graph graph;
	Parser prs(iss);
	prs.parse(graph);

	Dijkstra DAlg(graph);
	DAlg.calculate();
	DIter dit(DAlg);

	ostringstream oss;
	Printer prt(oss);
	prt.print(dit);

	ASSERT_EQ(oss.str(), full_output);
}