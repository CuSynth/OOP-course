#include "gtest/gtest.h"
#include "../Radix/Radix.hpp"

#include <type_traits>
#include <random>
#include <vector>
#include <array>
#include <deque>
#include <map>

#define TEST_SIZE 10000
#define DEFAULT_SEED 0 //std::random_device{}() 
#define ABS_LAMBDA [](int a, int b) { return abs(a) > abs(b); }

class myNumber {
public:
	myNumber(int _a = 0) : a(_a) {};

	bool operator<(const myNumber& rhs) { return this->a > rhs.a; }
	myNumber& operator=(int _a) { a = _a; return *this; }
	int num() const { return a; }

private:
	int a;
};
//std::ostream& operator<<(std::ostream& out, const myNumber& rhs) { out << rhs.num(); return out; }

template<class T, typename vtype = std::iterator_traits<T>::value_type>
typename std::enable_if_t<	std::is_integral_v<vtype> 
							&& !( std::is_same_v<vtype, char> || std::is_same_v<vtype, signed char> 
									|| std::is_same_v<vtype, unsigned char> || std::is_same_v<vtype, char16_t>)>
getRandomV(T begin, T end, vtype lower = std::numeric_limits<vtype>::lowest(), vtype upper = std::numeric_limits<vtype>::max()) {

	std::default_random_engine gen{ DEFAULT_SEED };
	std::uniform_int_distribution<vtype> distrib(lower, upper);
	std::generate(begin, end, [&gen, &distrib]() { return distrib(gen); });
}

template<class T, typename vtype = std::iterator_traits<T>::value_type>
typename std::enable_if_t<!std::is_integral_v<vtype>>
	getRandomV(T begin, T end, vtype lower = std::numeric_limits<vtype>::lowest()/2, vtype upper = std::numeric_limits<vtype>::max()/2) {
	// "uniform_real_distribution Requires that a ≤ b and b-a ≤ std::numeric_limits<RealType>::max()" => g дефолту берем /2

	std::default_random_engine gen{ DEFAULT_SEED };
	std::uniform_real_distribution<vtype> distrib(lower, upper);	
	std::generate(begin, end, [&gen, &distrib]() { return distrib(gen); });
}

template<class T, typename vtype = std::iterator_traits<T>::value_type>
typename std::enable_if_t<std::is_same_v<vtype, char> 
						|| std::is_same_v<vtype, signed char> 
						|| std::is_same_v<vtype, unsigned char>
						|| std::is_same_v<vtype, char16_t>>
	getRandomV(T begin, T end, vtype lower = std::numeric_limits<vtype>::lowest(), vtype upper = std::numeric_limits<vtype>::max()) {

	std::default_random_engine gen{ DEFAULT_SEED };
	std::uniform_int_distribution<> distrib(lower, upper);	// Генерируем от lower до upper, приведение не нужно?
	std::generate(begin, end, [&gen, &distrib]() { return distrib(gen); });
}



//TEST(NotCompile, NC) {
//	std::map<int, int> data{ {1,1}, {2,2}, {3,3} };
//	
//	radixSort(data.begin(), data.end(), [](int a, int b) { return a > b; });	// ERROR!
//
//	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()), [](int a, int b) { return a > b; });
//
//}

TEST(vectorSort, int_Vector) {
	typedef int T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());	

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));

}

TEST(vectorSort, int8_t_Vector) {
	typedef int8_t T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, uint8_t_Vector) {
	typedef uint8_t T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, char_Vector) {
	typedef char T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, unsigned_char_Vector) {
	typedef unsigned char T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, char16_t_Vector) {
	typedef char16_t T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, short_Vector) {
	typedef short T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, long_long_Vector) {
	typedef long long T;

	std::vector<T> data = { 1ll << 32,2ll << 32, 5ll << 32, 4ll << 32, 1ll << 32 };

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, ulong_long_Vector) {
	typedef unsigned long long T;

	std::vector<T> data = { 1ll << 32,2ll << 32, 5ll << 32, 4ll << 32, 1ll << 32 };


	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, uint_Vector) {
	typedef unsigned int T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, ulong_long_Vector_large) {
	typedef unsigned long long T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());
	for (auto& v : data)
		v = v & ~0xffffffffll;

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, long_long_Vector_maxmin) {
	typedef long long T;

	std::vector<T> data{ std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max(),
						std::numeric_limits<T>::lowest() + 1, std::numeric_limits<T>::max() - 1,
						std::numeric_limits<T>::lowest() + 2, std::numeric_limits<T>::max() - 2 };

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(vectorSort, ulong_long_Vector_maxmin) {
	typedef unsigned long long T;

	std::vector<T> data{ std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max(),
						std::numeric_limits<T>::lowest() + 1, std::numeric_limits<T>::max() - 1,
						std::numeric_limits<T>::lowest() + 2, std::numeric_limits<T>::max() - 2 };

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}



TEST(dequeSort, long_long_Deque) {
	typedef long long T;

	std::deque<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(arrSort, long_long_Arr) {
	typedef long long T;

	std::array<T, TEST_SIZE> data;
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(arrSort, long_long_CArr) {
	typedef long long T;

	T v[TEST_SIZE];
	getRandomV(v, v + TEST_SIZE);

	radixSort(v, v + TEST_SIZE);
	EXPECT_TRUE(std::is_sorted(v, v + TEST_SIZE));
}



TEST(floatSort, float_Vector) {
	typedef float T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(notDefaultComp, UtoL) {
	typedef int T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end(), [](int a, int b) { return (a) > (b); });
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), [](int a, int b) { return (a) > (b); }));
}

TEST(notDeaultData, myNum) {
	std::vector<myNumber> data{ -1, 3, -5, 2, -10, -15, -123, -2, -2, 5 };

	radixSort(data.begin(), data.end());
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(notDefaultComp, absSort) {
	typedef int T;

	std::vector<T> data(TEST_SIZE);
	getRandomV(data.begin(), data.end());

	radixSort(data.begin(), data.end(), ABS_LAMBDA);
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), ABS_LAMBDA));
}
