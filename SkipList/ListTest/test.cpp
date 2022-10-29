#include "gtest/gtest.h"
#include <functional>
#include <fstream>

#include "../SkipList/skipList.hpp"
#include "serializer.hpp"

#define TEST_COUNT 30000

using namespace std;

class customData {
	int data{};

public:
	customData() = default;
	customData(int _data) : data(_data) {}

	bool operator==(const customData& rhs) const { return data == rhs.data; }
	bool operator!=(const customData& rhs) const { return data != rhs.data; }
	bool operator<(const customData& rhs) const { return data < rhs.data; }

};

template<typename T>
struct custom_comparator {
	constexpr bool operator()(const T& lhs, const T& rhs) const {
		return lhs > rhs;
	}
};

template <typename T>
struct custom_allocator {
	static void* allocate(const size_t count) {
		return operator new(count * sizeof(T));
	}

	static void deallocate(T* ptr, const size_t count) {
		operator delete(static_cast<void*>(ptr));
	}
};

TEST(custom, custom_data_key) {
	skip_list<customData, string> lst;
	lst[customData(10)] = "ten";
	lst[customData(1)] = "one";

	ASSERT_EQ(lst.size(), 2);
	ASSERT_EQ(lst.begin()->second, "one");
	ASSERT_EQ((++lst.begin())->second, "ten");
}

TEST(custom, custom_data_val) {
	skip_list<int, customData> lst;
	lst[10] = customData(-10);
	lst[1] = customData(-1);

	ASSERT_EQ(lst.size(), 2);
	ASSERT_EQ(lst.begin()->second, customData(-1));
	ASSERT_EQ((++lst.begin())->second, customData(-10));
}


TEST(serislization, simple) {
	skip_list<int, string> lst;
	lst[1] = "one";
	lst[2] = "two";
	lst[3] = "three";

 	ostringstream ofs(ofstream::out | ofstream::binary);
	{
		auto _data = lst;
		serialize(_data, ofs);
	}

	skip_list<int, string> nlst;
	istringstream ifs{ ofs.str(), ifstream::in | ofstream::binary };
	ifs >> noskipws;
	deserialize(nlst, ifs);

	ASSERT_EQ(lst, nlst);
}


TEST(ctr, empty) {
	skip_list<int, int> lst;

	ASSERT_TRUE(lst.empty());
	ASSERT_EQ(lst.size(), 0);
	ASSERT_TRUE(lst.begin() == lst.end());
}

TEST(ctr, _copy) {
	skip_list<int, int> lst;
	lst[1] = 2;
	auto nlst = skip_list<int, int>(lst);

	ASSERT_TRUE(!nlst.empty());
	ASSERT_EQ(nlst.size(), 1);
	ASSERT_TRUE(lst.begin() != lst.end());
	ASSERT_EQ(nlst[1], 2);
}

TEST(ctr, std_greater) {
	skip_list<int, int, std::greater<int>> lst;
	lst[1] = 2;
	lst[10] = 20;

	ASSERT_TRUE(!lst.empty());
	ASSERT_EQ(lst.size(), 2);
	ASSERT_TRUE(lst.begin() != lst.end());

	ASSERT_EQ(lst.begin()->second, 20);
	ASSERT_EQ((++lst.begin())->second, 2);
}

TEST(ctr, custom_comparator_allocator) {
	skip_list<int, int, custom_comparator<int>, custom_allocator<pair<const int, int>>> lst;
	lst[1] = 2;

	ASSERT_TRUE(!lst.empty());
	ASSERT_EQ(lst.size(), 1);
	ASSERT_TRUE(lst.begin() != lst.end());
	ASSERT_EQ(lst[1], 2);
}

TEST(_operator, insert_size_empty) {
	skip_list<int, int> lst;

	ASSERT_TRUE(lst.empty());
	ASSERT_EQ(lst.size(), 0);
	
	ASSERT_TRUE((lst.insert(make_pair(1, 2))).second);
	ASSERT_TRUE((lst.insert(make_pair(3, 4))).second);

	ASSERT_TRUE(!lst.empty());
	ASSERT_EQ(lst.size(), 2);

	ASSERT_EQ(lst[1], 2);
	ASSERT_EQ(lst[3], 4);
	ASSERT_TRUE(lst.begin() != lst.end());

	ASSERT_FALSE((lst.insert(make_pair(1, 222))).second);
	
	ASSERT_EQ(lst[1], 2);
	ASSERT_EQ(lst.size(), 2);
}

TEST(_operator, brackets_insert) {
	skip_list<int, int> lst;
	lst[1] = 2;
	lst[2] = 3;

	ASSERT_EQ(lst[1], 2);
	ASSERT_EQ(lst[2], 3);
}

TEST(_operator, brackets_change) {
	skip_list<int, int> lst;

	lst.insert(make_pair(1, 2));
	lst.insert(make_pair(2, 3));
	lst.insert(make_pair(5, 6));

	lst[1] = -2;
	lst[2] = -3;
	lst[5] = -6;

	ASSERT_EQ(lst[1], -2);
	ASSERT_EQ(lst[2], -3);
	ASSERT_EQ(lst[5], -6);
}

TEST(_operator, eq) {
	skip_list<int, int> lst;
	skip_list<int, int> nlst;

	lst[1]= 2;
	nlst[1] = 2;

	ASSERT_TRUE(lst == nlst);
}

TEST(_operator, not_eq) {
	skip_list<int, int> lst;
	skip_list<int, int> nlst;

	lst[1] = 2;
	nlst[1] = -2;

	ASSERT_TRUE(lst != nlst);
}

TEST(_operator, at) {
	skip_list<int, int> lst;
	lst[1] = 2;
	lst[-1] = -2;

	ASSERT_THROW(lst.at(0), out_of_range);

	ASSERT_EQ(lst.at(1), 2);
	lst.at(1) = 1;
	ASSERT_EQ(lst.at(1), 1);
}


TEST(_operator, _copy) {
	skip_list<int, int> lst;
	skip_list<int, int> nlst;

	lst[1] = 2;
	lst[2] = 3;
	lst[5] = 6;

	nlst = lst;
	ASSERT_TRUE(nlst == lst);

	nlst[1] = -2;
	nlst[2] = -3;
	nlst[5] = -6;

	ASSERT_FALSE(nlst == lst);
}

TEST(_operator, erase_by_position) {
	skip_list<int, int> lst;

	lst[1] = 2;
	lst[2] = 3;
	lst[5] = 6;

	auto it = lst.begin();

	lst.erase(++it);

	ASSERT_EQ(lst.find(2), lst.end());
	ASSERT_EQ(lst.size(), 2);
	ASSERT_EQ(lst[1], 2);
	ASSERT_EQ(lst[5], 6);
}

TEST(_operator, erase_by_key) {
	skip_list<int, int> lst;

	lst[1] = 2;
	lst[2] = 3;
	lst[5] = 6;

	lst.erase(1);

	ASSERT_EQ(lst.find(1), lst.end());
	ASSERT_EQ(lst.size(), 2);
	ASSERT_EQ(lst[2], 3);
	ASSERT_EQ(lst[5], 6);
}

TEST(_operator, _find) {
	skip_list<int, int> lst;
	lst[1] = 2;

	ASSERT_EQ((lst.find(1))->second, 2);
	ASSERT_EQ(lst.find(-1), lst.end());
}

TEST(_operator, erase_by_distance) {
	skip_list<int, int> lst;

	lst[1] = 2;
	lst[2] = 3;
	lst[5] = 6;
	lst[10] = 0;
	lst[15] = -10;

	auto _begin = lst.begin();
	auto _end = lst.find(10);

	lst.erase(++_begin, _end);

	ASSERT_EQ(lst.find(2), lst.end());
	ASSERT_EQ(lst.find(5), lst.end());

	ASSERT_EQ(lst.size(), 3);
	ASSERT_EQ(lst[1], 2);
	ASSERT_EQ(lst[10], 0);
	ASSERT_EQ(lst[15], -10);
}

TEST(_operator, clear) {
	skip_list<int, int> lst;

	lst[1] = 2;
	lst[2] = 3;
	lst[5] = 6;
	lst.clear();

	ASSERT_EQ(lst.size(), 0);
	ASSERT_TRUE(lst.empty());
	ASSERT_EQ(lst.begin(), lst.end());
}

TEST(_operator, swap) {
	skip_list<int, int> lst;
	lst[1] = 2;
	lst[2] = 3;
	lst[5] = 6;

	skip_list<int, int> nlst;
	nlst[1] = -2;
	nlst[2] = -3;

	lst.swap(nlst);

	ASSERT_EQ(lst.size(), 2);
	ASSERT_EQ(nlst.size(), 3);

	ASSERT_EQ(lst[1], -2);
	ASSERT_EQ(nlst[1], 2);
}

TEST(iter, begin_end) {
	skip_list<int, int> lst;
	lst[1] = 2;
	lst[2] = 3;

	auto it = lst.begin();

	ASSERT_EQ(it->second, 2);
	it->second = -2;
	ASSERT_EQ(it->second, -2);

	ASSERT_EQ((++it)->second, 3);
	ASSERT_EQ(++it, lst.end());
	ASSERT_THROW(++it, out_of_range);
}

TEST(iter, order) {
	skip_list<int, int> lst;

	for (int i = 0; i < TEST_COUNT; i++) {
		lst[i] = TEST_COUNT - i;	// [30..1]
	}

	int j = TEST_COUNT;
	for (auto& it = lst.begin(); it != lst.end(); ++it, --j) {
		ASSERT_EQ(it->second, j);
	}
}

TEST(riter, begin_end) {
	skip_list<int, int> lst;
	lst[1] = 2;
	lst[2] = 3;

	auto it = lst.rbegin();

	ASSERT_EQ(it->second, 3);
	it->second = -3;
	ASSERT_EQ(it->second, -3);

	ASSERT_EQ((++it)->second, 2);
	ASSERT_EQ(++it, lst.rend());
	ASSERT_THROW(++it, out_of_range);
}

TEST(riter, order) {
	skip_list<int, int> lst;

	for (int i = 0; i < TEST_COUNT; i++) {
		lst[i] = TEST_COUNT - i;	// [30..1]
	}

	int j = 1;
	for (auto& it = lst.rbegin(); it != lst.rend(); ++it, ++j) {
		ASSERT_EQ(it->second, j);
	}
}