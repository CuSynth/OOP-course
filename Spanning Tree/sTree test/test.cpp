#include "gtest/gtest.h"

#include "..\Spanning Tree\sTree.hpp"
#include "..\Spanning Tree\IO.hpp"
//#include "..\Spanning Tree\structs.hpp"

const char* simple_input = 
"1, 0, 0\n"
"2, 2, 0\n"
"3, 0, 1";

vertex_vector simple_expection = { {1, 0, 0}, {2, 2, 0}, {3, 0, 1} };

const char* simple_output =
"1 - 3\n"
"1 - 2\n";

const char* full_input =
"1, 7, 5\n"
"2, 3, -11\n"
"3, 2, -2\n"
"4, -1, -8\n"
"5, 14, -14\n"
"6, 5, -3";

const char* full_output =
"3 - 6\n"
"2 - 4\n"
"3 - 4\n"
"1 - 6\n"
"2 - 5\n";

bool operator==(const vertex& lhs, const vertex& rhs) {
	return lhs.id == rhs.id && lhs.x == rhs.x && lhs.y == rhs.y;
}

TEST(Input, GoodInput) {
	auto is = std::istringstream(simple_input);

	vertex_vector real = reader(is);

	ASSERT_EQ(simple_expection.size(), real.size());
	ASSERT_EQ(simple_expection, real);
}

TEST(Input, SpacedInput) {
	const char* spaced_input =
		"1, 0, 0\n"
		"			 \n"
		"	\n"
		" \n"
		"2, 2,     0\n"
		"3    ,			0    , 1";
	auto is = std::istringstream(spaced_input);

	vertex_vector real = reader(is);

	ASSERT_EQ(simple_expection.size(), real.size());
	ASSERT_EQ(simple_expection, real);
}

TEST(Input, IncorrectInput) {
	const char* incorrect_input =
		"1, 2, 3\n"
		"qwer 5, 6";
	auto is = std::istringstream(incorrect_input);

	ASSERT_THROW(reader(is), std::invalid_argument);
}

TEST(Output, SimpleOutput) {
	mst toPrt = { {1, 2}, {3, 4} };
	auto os = std::ostringstream();
	os << toPrt;

	ASSERT_EQ(os.str(), "1 - 2\n3 - 4\n");
}

TEST(Ctr, EmptyCtr) {
	MinSpanningTree tree;
}

TEST(Ctr, Create) {
	auto is = std::istringstream(simple_input);
	vertex_vector vxs = reader(is);

	MinSpanningTree tree(vxs);
}

TEST(Ctr, CreateAndInit) {
	auto is = std::istringstream(simple_input);
	vertex_vector vxs = reader(is);

	MinSpanningTree tree;
	tree.init(vxs);
}

TEST(Ctr, EmptyTree) {
	MinSpanningTree tree;
	mst newMst= tree.Kruskal();

	ASSERT_TRUE(newMst.empty());
}

TEST(Kruskal, Simple) {
	auto is = std::istringstream(simple_input);
	auto os = std::ostringstream();

	vertex_vector vertexes = reader(is);
	MinSpanningTree tree(vertexes);
	mst newMst = tree.Kruskal();
	os << newMst;

	ASSERT_STREQ(os.str().data(), simple_output);
}

TEST(Kruskal, Full) {
	auto is = std::istringstream(full_input);
	auto os = std::ostringstream();

	vertex_vector vertexes = reader(is);
	MinSpanningTree tree(vertexes);
	mst newMst = tree.Kruskal();
	os << newMst;

	ASSERT_STREQ(os.str().data(), full_output);
}

TEST(ReUse, ReInit) {
	auto is = std::istringstream(simple_input);
	auto os = std::ostringstream();
	

	vertex_vector vertexes = reader(is);
	MinSpanningTree tree(vertexes);
	mst newMst = tree.Kruskal();
	os << newMst;

	ASSERT_STREQ(os.str().data(), simple_output);

	os.str("");
	os.clear();
	is.clear();
	is = std::istringstream(full_input);
	
	vertexes = reader(is);
	tree.init(vertexes);
	newMst = tree.Kruskal();
	os << newMst;

	ASSERT_EQ(os.str(), full_output);
}

TEST(ReUse, ReKruskal) {
	auto is = std::istringstream(simple_input);
	auto os = std::ostringstream();
	MinSpanningTree tree;

	vertex_vector vertexes = reader(is);
	tree.init(vertexes);
	mst newMst = tree.Kruskal();
	os << newMst;
	ASSERT_STREQ(os.str().data(), simple_output);
	
	for (size_t i = 0; i < 5; i++) {
		newMst = tree.Kruskal();
		
		os.str("");
		os.clear(); 
		os << newMst;
		
		ASSERT_EQ(os.str(), simple_output);
	}
}