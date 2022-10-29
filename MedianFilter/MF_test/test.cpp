#include "gtest/gtest.h"

#include <sstream>

#include "../MedianFilter/IO.hpp"
#include "../MedianFilter/MedianFilter.hpp"

using namespace std;

string simpleIF = "IF.bmp 3 1 OF.bmp";
string spacedIF = "IF.bmp \n3				1   OF.bmp";
RawData IFExp = { "IF.bmp", "OF.bmp", 3, 1 };	// Используется также и в проверке фильтрации открытого изображения

pxVector simpleInput = {{1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 0},
						{6, 0, 0}, {7, 0, 0}, {8, 0, 0}, {9, 0, 0}, {10, 0, 0},
						{11, 0, 0}, {12, 0, 0}, {13, 0, 0}, {14, 0, 0}, {15, 0, 0},
						{16, 0, 0}, {17, 0, 0}, {18, 0, 0}, {19, 0, 0}, {20, 0, 0},
						{21, 0, 0}, {22, 0, 0}, {23, 0, 0}, {24, 0, 0}, {25, 0, 0}
};
size_t SIWidth = 5;
size_t SIHeight = 5;

// Посчитано руками:
pxVector simpleExp = {	{6, 0, 0}, {6, 0, 0}, {7, 0, 0}, {8, 0, 0}, {5, 0, 0},
						{6, 0, 0}, {7, 0, 0}, {8, 0, 0}, {9, 0, 0}, {10, 0, 0},
						{11, 0, 0}, {12, 0, 0}, {13, 0, 0}, {14, 0, 0}, {15, 0, 0},
						{16, 0, 0}, {17, 0, 0}, {18, 0, 0}, {19, 0, 0}, {20, 0, 0},
						{21, 0, 0}, {22, 0, 0}, {23, 0, 0}, {24, 0, 0}, {25, 0, 0}
};

bool operator==(const RawData& lhs, const RawData& rhs) {
	return lhs.IFName == rhs.IFName
		&& lhs.OFName == rhs.OFName
		&& lhs.size == rhs.size
		&& lhs.level == rhs.level;
}

bool operator==(const bmp::Pixel& lhs, const bmp::Pixel& rhs) {
	return lhs.r == rhs.r
		&& lhs.g == rhs.g
		&& lhs.b == rhs.b;
}

bool cmp(const pxVector& f, const pxVector& s) {
	if (f.size() != s.size())
		return false;

	for (size_t i = 0; i < f.size(); i++) {
		if (!(f[i] == s[i]))
			return false;
	}
	return true;
}

TEST(Input, GoodInput) {
	auto is = istringstream(simpleIF);
	RawData dat = reader(is);

	ASSERT_EQ(IFExp, dat);
}

TEST(Input, SpacedInput) {
	auto is = istringstream(spacedIF);
	RawData dat = reader(is);

	ASSERT_EQ(IFExp, dat);
}

TEST(Input, IncorrectInput) {
	string incorrect_input =
		"name.txt C 123 23 NotName";
	auto is = istringstream(incorrect_input);

	ASSERT_THROW(reader(is), invalid_argument);
}

TEST(Ctr, EmptyCtr) {
	MedianFilter filter;
}

TEST(Ctr, EmptyCtrData) {
	MedianFilter filter;
	ASSERT_EQ(filter.get_result().data().size(), 0);
}

TEST(Filter, FilterFromVector) {
	MedianFilter filter;
	filter.filter(simpleInput, SIWidth, SIHeight, 3, 1);
	
	bmp::Bitmap newImg = filter.get_result();
	ASSERT_EQ(newImg.Width(), SIWidth);
	ASSERT_EQ(newImg.Height(), SIHeight);

	ASSERT_TRUE(cmp(newImg.data(), simpleExp));
	//ASSERT_EQ(newImg.data(), simple_data);	// Не работает (?)
}

TEST(Filter, FilterFromImg) {
	bmp::Bitmap orig, filtered, dest;
	orig.Load(IFExp.IFName);
	filtered.Load(IFExp.OFName);

	MedianFilter filter;
	filter.filter(orig, 3, 1);
	dest = filter.get_result();

	ASSERT_EQ(dest.Width(), filtered.Width());
	ASSERT_EQ(dest.Height(), filtered.Height());
	ASSERT_TRUE(cmp(dest.data(), filtered.data()));
}

TEST(Filter, WrongSizeLevel) {
	bmp::Bitmap tmp;
	MedianFilter filter;
	
	ASSERT_THROW(filter.filter(tmp, 3, 5), out_of_range);
}