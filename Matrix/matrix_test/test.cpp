#include "gtest/gtest.h"
#include "../Matrix/matrix.hpp"

using namespace std;
using imt = Matrix<int>;

constexpr size_t diag_size = 100;

const vector<vector<int>> not_data{
	{1, 3, 5, 7, 9, 13},
	{7, 5, 3, 1},
	{1, 2}
};

const vector<vector<int>> sq_data{
	{1, 3, 5},
	{7, 5, 3},
	{1, 2, 3}
};

const vector<vector<int>> rect_data{
	{1, 3, 5, 7, 13},
	{7, 5, 3, 1, -1},
	{1, 2, 3, 4, 5}
};

const vector<vector<int>> rect_data_T{
	{1, 7, 1},
	{3, 5, 2},
	{5, 3, 3},
	{7, 1, 4},
	{13, -1, 5}
};

const vector<vector<int>> diag_data{
	{1, 0, 0},
	{0, 2, 0},
	{0, 0, 3}
};

bool operator==(const vector<vector<int>>& l, const vector<vector<int>>& r) {
	size_t sz = l.size();
	if (sz != r.size())
		return false;

	for (size_t i = 0; i < sz; ++i) {
		size_t _sz = l.size();
		if (_sz != r[i].size())
			return false;

		for (size_t j = 0; j < _sz; ++j) {
			if (l[i][j] != r[i][j])
				return false;
		}
	}

	return true;
}


template<class T>
Matrix<T> diagonal(size_t n, T d = 1, T val = 0)
{
	Matrix<T> m{ n, n };
	for (size_t i = 0; i < m.rows_count(); i++)
		for (size_t k = 0; k < m.cols_count(); k++)
			if (i != k) m.at(i, k) = val;
			else m.at(i, k) = d;
	return m;
}


TEST(ctr, zero_size) {
	EXPECT_THROW(imt m(0, 0), std::invalid_argument);
}

TEST(ctr, by_size) {
	imt m(3, 5);
	pair<size_t, size_t>  _dim = { 3, 5 };
	ASSERT_EQ(m.dim(), _dim);
}

TEST(ctr, by_vector_from_rhs) {
	imt m({ {1, 2}, {3, 4} });
	pair<size_t, size_t>  _dim = { 2, 2 };
	ASSERT_EQ(m.dim(), _dim);

	vector<vector<int>> exp{ {1, 2}, {3, 4} };
	ASSERT_EQ(m.raw_data(), exp);
}

TEST(ctr, square_by_vector) {
	imt m(sq_data);
	pair<size_t, size_t>  _dim = { 3, 3 };
	ASSERT_EQ(m.dim(), _dim);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(ctr, rect_by_vector) {
	imt m(rect_data);
	pair<size_t, size_t>  _dim = { 3, 5 };
	ASSERT_EQ(m.dim(), _dim);
	ASSERT_EQ(m.raw_data(), rect_data);
}

TEST(ctr, diag_by_vector) {
	imt m(diag_data);
	pair<size_t, size_t>  _dim = { 3, 3 };
	ASSERT_EQ(m.dim(), _dim);
	ASSERT_EQ(m.raw_data(), diag_data);
}


TEST(ctr, by_err_vector) {
	ASSERT_THROW(imt m(not_data), invalid_argument);
}

TEST(at, simle) {
	imt m(sq_data);

	ASSERT_EQ(m.at(1, 1), 5);
	ASSERT_THROW(m.at(5, 10), out_of_range);
}


TEST(add, square_sum) {
	imt m(sq_data);
	imt new_matrix(3, 3);

	new_matrix = m + m;

	vector<vector<int>> exp(3);
	for (size_t i = 0; i < 3; ++i) {
		exp[i].resize(3);
		for (size_t j = 0; j < 3; ++j) {
			exp[i][j] = sq_data[i][j] * 2;
		}
	}

	ASSERT_EQ(new_matrix.raw_data(), exp);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(add_assum, square_sum) {
	imt m(sq_data);
	imt new_matrix(sq_data);

	new_matrix += m;

	vector<vector<int>> exp(3);
	for (size_t i = 0; i < 3; ++i) {
		exp[i].resize(3);
		for (size_t j = 0; j < 3; ++j) {
			exp[i][j] = sq_data[i][j] * 2;
		}
	}

	ASSERT_EQ(new_matrix.raw_data(), exp);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(sub, square_sub) {
	imt m(sq_data);
	imt new_matrix(3, 3);

	new_matrix = m - m;

	vector<vector<int>> exp(3);
	for (size_t i = 0; i < 3; ++i) {
		exp[i].resize(3);
		for (size_t j = 0; j < 3; ++j) {
			exp[i][j] = 0;
		}
	}

	ASSERT_EQ(new_matrix.raw_data(), exp);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(sub_assum, square_sub) {
	imt m(sq_data);
	imt new_matrix(m.rows_count(), m.cols_count());

	new_matrix -= m;

	vector<vector<int>> exp(3);
	for (size_t i = 0; i < 3; ++i) {
		exp[i].resize(3);
		for (size_t j = 0; j < 3; ++j) {
			exp[i][j] = (-1) * sq_data[i][j];
		}
	}

	ASSERT_EQ(new_matrix.raw_data(), exp);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(mult, sq_mult) {
	imt m(sq_data);
	imt new_matrix(m.rows_count(), m.cols_count());

	new_matrix = m * m;

	vector<vector<int>> exp = { {27, 28, 29},
								{45, 52, 59},
								{18, 19, 20} };

	ASSERT_EQ(new_matrix.raw_data(), exp);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(mult_assum, sq_mult) {
	imt m(sq_data);
	imt new_matrix(m);

	new_matrix *= m;

	vector<vector<int>> exp = { {27, 28, 29},
								{45, 52, 59},
								{18, 19, 20} };

	ASSERT_EQ(new_matrix.raw_data(), exp);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(mult_assum, rect_mult) {
	imt m(rect_data_T);
	imt new_matrix(rect_data);

	new_matrix *= m;

	vector<vector<int>> exp = { {253, 31, 115},
								{31, 85, 25},
								{115, 25, 55} };

	ASSERT_EQ(new_matrix.raw_data(), exp);
	ASSERT_EQ(m.raw_data(), rect_data_T);
}

TEST(det, simple_det) {
	imt m(sq_data);
	ASSERT_EQ(m.det(), 0.0);
	ASSERT_EQ(m.raw_data(), sq_data);
}

TEST(det, another_det) {
	vector<vector<double>> _dat = {
		{ -5, 19, 0.5, -3},
		{ 1.1, 23, -53.2, 13},
		{ -23, 3, -65.3, -15},
		{ 9, 0.045, 0, 0}
	};
	Matrix<double> m(_dat);
	ASSERT_NEAR(m.det(), -319792.0257, 0.00001);
	ASSERT_EQ(m.raw_data(), _dat);
}




TEST(det, double_diag) {
	Matrix<double> m = diagonal<double>(diag_size, 0.0, 1.0);

	double res = ((double)(diag_size & 1) != 0 ? 1 : -1) * ((double)diag_size - 1);
	ASSERT_NEAR(m.det(), res, 0.00001);
}

TEST(mult, double_diag) {
	Matrix<double> a = diagonal<double>(diag_size, 0.0, 1.0);
	Matrix<double> b(a);
	Matrix<double> res(diag_size, diag_size);
	Matrix<double> exp = diagonal<double>(diag_size, (double)(diag_size - 1), (double)(diag_size - 2));

	res = a * b;
	ASSERT_EQ(res, exp);
}

TEST(sum, double_diag) {
	Matrix<double> a = diagonal<double>(diag_size);
	Matrix<double> b = diagonal<double>(diag_size, -1.0, 3.0);
	Matrix<double> res(diag_size, diag_size);
	Matrix<double> exp = diagonal<double>(diag_size, 0.0, 3.0);

	res = a + b;
	ASSERT_EQ(res, exp);
}

TEST(sub, double_diag) {
	Matrix<double> a = diagonal<double>(diag_size);
	Matrix<double> b = diagonal<double>(diag_size, 1.0, 5.0);
	Matrix<double> res(diag_size, diag_size);
	Matrix<double> exp = diagonal<double>(diag_size, 0.0, -5.0);

	res = a - b;
	ASSERT_EQ(res, exp);
}
