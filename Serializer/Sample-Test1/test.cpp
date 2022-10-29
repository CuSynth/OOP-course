#include "gtest/gtest.h"
#include "../Serializer/serializer.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <forward_list>
#include <list>
#include <deque>
#include <set>

#define EPS 1

using namespace std;

bool fcmp(float a, float b) {
	return abs(abs(a) - abs(b)) < EPS;
}

struct Person {
	std::string name;
	std::vector<float> id;
	std::map<std::string, std::string> motherland;
	friend bool operator==(const Person& left, const Person& right);
};

bool operator==(const Person& left, const Person& right) {
	return left.name == right.name
		&& equal(left.id.begin(), left.id.end(), right.id.begin(), fcmp)
		&& left.motherland == right.motherland;
}

template <>
struct serializer<Person> {
	static void apply(const Person& obj, std::ostream& os) {
		serialize(obj.name, os);
		serialize(obj.id, os);
		serialize(obj.motherland, os);
	}
};

template <>
struct deserializer<Person> {
	static void apply(Person& val, std::istream& is) {
		deserialize(val.name, is);
		deserialize(val.id, is);
		deserialize(val.motherland, is);
	}
};

TEST(person, just_a_single_person) {
	Person man{ "Vasya", 
				{256.0f, 0.0f, 128.0f}, 
				{{"dad", "mom"}, {"granddad_1", "grandmom_1"}, {"granddad_1", "grandmom_1"}} };

	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _man = man;
		serialize(_man, ofs);
	}

	Person notAHuman;
	istringstream ifs{ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(notAHuman, ifs);

	EXPECT_TRUE(man == notAHuman);
}

TEST(person, crowd) {
	vector<Person> crowd{ { "Vasya",
				  {256.0f, 0.0f, 128.0f},
				  {{"dad", "mom"}, {"granddad_1", "grandmom_1"}, {"granddad_1", "grandmom_1"}} },

				  { "nonVasya",
				  {255.0f, 1.0f, 64.0f},
				  {{"dad", "mom"}, {"granddad_1", "grandmom_1"}, {"granddad_1", "grandmom_1"}} },

				  { "absolutelyNonVasya",
				  {254.0f, 2.0f, 32.0f},
				  {{"dad", "mom"}, {"granddad_1", "grandmom_1"}, {"granddad_1", "grandmom_1"}} } };


	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _crowd = crowd;
		serialize(_crowd, ofs);
	}

	vector<Person> justAnotherCrowd;
	istringstream ifs{ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(justAnotherCrowd, ifs);

	EXPECT_TRUE(crowd == justAnotherCrowd);
}



TEST(simple, int_check) {
	int data = 128;
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	int nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_TRUE(data == nData);
}

TEST(simple, _char) {
	char data = 'a';
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	char nData;
	istringstream ifs{ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_TRUE(nData == data);
}

TEST(simple, cStr) { 
	char data[7] = "hello!";
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		char _data[7];
		std::copy(data, data + 7, _data);
		serialize(_data, ofs);
	}

	char nData[7];
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_STREQ(nData, data);
}

TEST(simple, cArr) {
	int data[3] = { 1, 2, 3 };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		int _data[3];
		std::copy(data, data + 3, _data);
		serialize(_data, ofs);
	}

	int nData[3];
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	for (size_t i = 0; i < 3; ++i)
		EXPECT_EQ(nData[i], data[i]);
}

TEST(simple, str) {
	string data = "Hello world!";
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	string nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(vect, intVector) {
	vector<int> data = { 1, 2, 3, 4 };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	vector<int> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(vect, charVector) {
	vector<char> data = { 'a', 'b', 'c', 'd'};
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	vector<char> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(vect, strVector) {
	vector<std::string> data = { "Hello_1", "Hello_2", "Hello_3", "Hello_4" };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	vector<std::string> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(map, int_int_map) {
	map<int, int> data = { {1, 1}, {2, 2}, {3, 3} };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	map<int, int> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(_map, int_char_map) {
	map<int, char> data = { {1, 'a'}, {2, 'b'}, {3, 'c'}};
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	map<int, char> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(fwd_list, int_list) {
	forward_list<int> data{ 1, 2, 3, 4 };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	forward_list<int> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);
	
	EXPECT_EQ(data, nData);
}

TEST(_list, int_list) {
	list<int> data{ 1, 2, 3, 4 };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	list<int>  nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(_deque, int_deque) {
	deque<int> data{ 1, 2, 3, 4 };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	deque<int>  nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(_set, int_set) {
	set<int> data{ 1, 2, 3, 4 };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	set<int>  nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(_multiset, int_set) {
	multiset<int> data{ 1, 1, 2, 2, 3, 3, 4, 4 };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	multiset<int>  nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}



TEST(multiContainer, vect_of_map_of_int_char) {
	std::vector<std::map<int, char>> data = { { {1, 'a'}, {2, 'b'}, {3, 'c'} },
											  { {1, 'd'}, {2, 'e'}, { 3, 'f'} } };
	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	std::vector<std::map<int, char>> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}

TEST(multiContainer, vect_of_map_of_int_and_set_of_deque_of_char) {
	std::vector<std::map<int, set<deque<char>>>> data = { { {1, { {'a', 'b', 'c'}}},
															{2, { {'d', 'e', 'f'}}},
															{3, { {'g', 'h', 'i'}}} },
														  { {1, { {'j', 'k', 'l'}}},
															{2, { {'m', 'n', 'o'}}},
															{3, { {'p', 'q', 'r'}}} } };

	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = data;
		serialize(_data, ofs);
	}

	std::vector<std::map<int, set<deque<char>>>> nData;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nData, ifs);

	EXPECT_EQ(data, nData);
}