#include "gtest/gtest.h"
#include "../bg_crop/bg_crop.hpp"

using namespace std;
using px_vector = vector<bmp::Pixel>;

bool operator==(const bmp::Pixel& lhs, const bmp::Pixel& rhs) {
	return lhs.r == rhs.r
		&& lhs.g == rhs.g
		&& lhs.b == rhs.b;
}

bool cmp(const px_vector& f, const px_vector& s) {
	if (f.size() != s.size())
		return false;

	for (size_t i = 0; i < f.size(); i++) {
		if (!(f[i] == s[i]))
			return false;
	}
	return true;
}

TEST(Ctrs, EmptyArg) {
	bmp::Bitmap img;
	Croper crp;
}

TEST(Crop, Default) {
	bmp::Bitmap simple_bmp(5, 5);
	for (size_t i = 0; i < 25; i++)
		simple_bmp[i] = { 0, 1, 3 };

	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			simple_bmp[(i + 1) * 5 + (j + 1)] = { 1, 5, 10 };

	bmp::Bitmap exp(3, 3);
	for (size_t i = 0; i < 9; i++)
		exp[i] = { 1, 5, 10 };

	Croper crp;
	crp.crop(simple_bmp);

	ASSERT_TRUE(cmp(crp.GetNewBitmap().data(), exp.data()));
}

TEST(Crop, SetPivot) {
	bmp::Bitmap simple_bmp(5, 5);
	for (size_t i = 0; i < 25; i++)
		simple_bmp[i] = { 0, 1, 3 };

	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			simple_bmp[(i + 1) * 5 + (j + 1)] = { 1, 5, 10 };

	bmp::Bitmap exp(3, 3);
	for (size_t i = 0; i < 9; i++)
		exp[i] = { 1, 5, 10 };

	Croper crp;
	crp.crop(simple_bmp);

	ASSERT_TRUE(cmp(crp.GetNewBitmap().data(), exp.data()));

	Croper crp2;
	crp2.setPivot(1, 1);
	crp2.crop(simple_bmp);
	ASSERT_TRUE(cmp(crp2.GetNewBitmap().data(), simple_bmp.data()));
}

TEST(Crop, SetEpsilon) {
	bmp::Bitmap simple_bmp(5, 5);
	for (size_t i = 0; i < 25; i++)
		simple_bmp[i] = { 0, 1, 3 };

	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			simple_bmp[(i + 1) * 5 + (j + 1)] = { 1, 5, 10 };

	bmp::Bitmap exp(3, 3);
	for (size_t i = 0; i < 9; i++)
		exp[i] = { 1, 5, 10 };

	Croper crp;
	crp.crop(simple_bmp);

	ASSERT_TRUE(cmp(crp.GetNewBitmap().data(), exp.data()));

	Croper crp2;
	crp2.setEpsilon(-1);
	crp2.crop(simple_bmp);
	ASSERT_TRUE(cmp(crp2.GetNewBitmap().data(), simple_bmp.data()));
}