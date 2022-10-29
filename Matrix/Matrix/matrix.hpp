#pragma once
#include <type_traits>
#include <vector>
#include <future>

template<typename T, typename _ = std::enable_if_t<std::is_arithmetic_v<T>>>
class Matrix final {
public:
	constexpr static size_t sync_threads = 1;

	constexpr static size_t det_threads = 5;
	constexpr static size_t det_pivot = 500;

	constexpr static size_t mult_threads = 6;
	constexpr static size_t mult_pivot = 200;

	constexpr static size_t oper_threads = 4;
	constexpr static size_t oper_pivot = 1000;

	using dim_t = std::pair<size_t, size_t>;

	Matrix() = delete;
	explicit Matrix(size_t r_num, size_t c_num);
	explicit Matrix(std::vector<std::vector<T>>&& dat);
	explicit Matrix(const std::vector<std::vector<T>>& dat);

	Matrix(Matrix&& m) = default;
	Matrix(const Matrix& m) = default;

	Matrix& operator=(const Matrix& m) = default;
	Matrix& operator=(Matrix&& m) noexcept = default;

	T& at(size_t r, size_t c) {
		return rows.at(r).at(c);
	}
	const T& at(const size_t row, const size_t c) const {
		return rows.at(row).at(c);
	}
	//T& operator[](size_t r, size_t c) {
	//	return rows.at(r).at(c);
	//}
	//const T& operator[](const size_t r, const size_t c) const {
	//	return rows.at(r).at(c);
	//}

	static void set_threads(size_t num) { autoThCount = false; thCount = num; }
	static void reset_threads() { autoThCount = true; }

	size_t rows_count() const {	return rows.size(); }
	size_t cols_count() const { return /*data_.empty() ? 0 :*/ rows[0].size(); }
	dim_t dim() const { return std::make_pair(rows_count(), cols_count()); }

	Matrix operator+(const Matrix& rhs) const;
	Matrix operator-(const Matrix& rhs) const;
	Matrix operator*(const Matrix& rhs) const;

	Matrix& operator+=(const Matrix& rhs);
	Matrix& operator-=(const Matrix& rhs);
	Matrix& operator*=(const Matrix& rhs);

	double det() const;

	bool operator==(const Matrix& rhs) const {
		return rows == rhs.rows;
	}

	bool operator!=(const Matrix& rhs) const {
		return rows != rhs.rows;
	}

	void clear() {
		rows.clear();
	}

	const std::vector<std::vector<T>>& raw_data() const { return rows; }

	size_t max_pos(size_t col) const;
	void swap(size_t i, size_t j) {
		std::swap(rows[i], rows[j]);
	}
	
	~Matrix() = default;

private:
	enum class oper { add, sub, mul, det };

	std::vector<std::vector<T>> rows{};
	inline static size_t thCount = 4;
	inline static bool autoThCount = true;

	static void set_th_count(oper op, dim_t dim);
	static void parallel_op(void (*op)(const Matrix&, const Matrix&, Matrix&, size_t, size_t),
		const Matrix& left, const Matrix& right, Matrix& result);
	static void double_copy(const Matrix& m_from, Matrix<double>& m_to);
	static void parallel_det(const Matrix& init_matrix, double& det);

	static void add(const Matrix& lhs, const Matrix& rhs, Matrix& res, size_t row_from, size_t row_to);
	static void sub(const Matrix& lhs, const Matrix& rhs, Matrix& res, size_t row_from, size_t row_to);
	static void mult(const Matrix& lhs, const Matrix& rhs, Matrix& res, size_t from, size_t to);
	static void copy(const Matrix& m_from, Matrix<double>& m_to, size_t r_from, size_t r_to);
	static void triang(Matrix<double>& mtx, size_t col, size_t r_from, size_t r_to);

};

template<typename T, typename _>
Matrix<T, _>::Matrix(size_t r_num, size_t c_num) 
{
	if (r_num == 0 || c_num == 0) {
		throw std::invalid_argument("Zero size matrix\n");
	}

	rows.resize(r_num);
	for (auto& it : rows) {
		it.resize(c_num);
	}
}

template<typename T, typename _>
Matrix<T, _>::Matrix(std::vector<std::vector<T>>&& dat) 
{
	if (dat.empty()) {
		throw std::invalid_argument("Zero size matrix init data\n");
	}

	size_t c_num = dat[0].size();
	for (auto& it : dat) {
		if (it.size() != c_num) {
			throw std::invalid_argument("Zero size matrix init data\n");
		}
	}

	rows = std::move(dat);
}

template<typename T, typename _>
Matrix<T, _>::Matrix(const std::vector<std::vector<T>>& dat)
{
	if (dat.empty()) {
		throw std::invalid_argument("Zero size matrix init data\n");
	}

	size_t r_num = dat[0].size();
	for (auto& it : dat) {
		if (it.size() != r_num) {
			throw std::invalid_argument("Wrong size matrix init data\n");
		}
	}

	rows = dat;
}

template<typename T, typename _>
void Matrix<T, _>::set_th_count(oper op, dim_t dim)
{
	if (autoThCount) {
		size_t r = dim.first;
		size_t c = dim.second;

		size_t mean = (r + c) / 2;
		switch (op) {
		case oper::mul:
			if (mean < mult_pivot)
				thCount = sync_threads;	// Sync
			else
				thCount = mult_threads;
			break;

		case oper::det:
			if (mean < det_pivot)
				thCount = sync_threads;	// Sync
			else
				thCount = det_threads;
			break;
		default:
			if (mean < oper_pivot)
				thCount = 1;	// Sync
			else
				thCount = oper_threads;
			break;
		}

	}
	thCount = thCount > std::thread::hardware_concurrency()
		? std::thread::hardware_concurrency() : thCount;
}

template<typename T, typename _>
void Matrix<T, _>::parallel_op(void (*op)(const Matrix&, const Matrix&, Matrix&, size_t, size_t),
	const Matrix& lhs, const Matrix& rhs, Matrix& res) 
{
	std::vector<std::future<void>> threads;
	for (size_t i = 0; i < thCount - 1; ++i) {
		size_t from = (i * lhs.rows_count() / thCount);
		size_t to = ((i + 1) * lhs.rows_count() / thCount);

		threads.push_back(std::async(op, std::ref(lhs), std::ref(rhs), std::ref(res), from, to));
	}
	size_t from = ((thCount - 1) * lhs.rows_count() / thCount);
	op(lhs, rhs, res, from, lhs.rows_count());

	for (auto& i : threads) {
		i.get();
	}
}

template<typename T, typename _>
Matrix<T, _> Matrix<T, _>::operator+(const Matrix& rhs) const 
{
	if (dim() != rhs.dim()) {
		throw::std::invalid_argument("Differen dimensions\n");
	}

	Matrix<T> res(*this);
	set_th_count(oper::add, dim());
	parallel_op(add, *this, rhs, res);

	return res;
}

template <typename T, typename _>
Matrix<T, _>& Matrix<T, _>::operator+=(const Matrix& rhs)
{
	if (dim() != rhs.dim()) {
		throw::std::invalid_argument("Differen dimensions\n");
	}

	set_th_count(oper::add, dim());
	parallel_op(add, rhs, rhs, *this);

	return *this;
}

template<typename T, typename _>
void Matrix<T, _>::add(const Matrix& lhs, const Matrix& rhs, Matrix& res, size_t row_from, size_t row_to)
{
	size_t cCount = lhs.cols_count();
	for (size_t i = row_from; i < row_to; ++i) {
		for (size_t j = 0; j < cCount; ++j) {
			res.rows[i][j] /*= lhs.rows[i][j]  */+= rhs.rows[i][j];
		}
	}
}

template<typename T, typename _>
Matrix<T, _> Matrix<T, _>::operator-(const Matrix& rhs) const
{
	if (dim() != rhs.dim()) {
		throw::std::invalid_argument("Differen dimensions\n");
	}

	Matrix res(*this);
	set_th_count(oper::sub, dim());
	parallel_op(sub, *this, rhs, res);

	return res;
}

template<typename T, typename _>
Matrix<T, _>& Matrix<T, _>::operator-=(const Matrix& rhs)
{
	if (dim() != rhs.dim()) {
		throw::std::invalid_argument("Differen dimensions\n");
	}

	set_th_count(oper::sub, dim());
	parallel_op(sub, rhs, rhs, *this);

	return *this;
}

template<typename T, typename _>
void Matrix<T, _>::sub(const Matrix& lhs, const Matrix& rhs, Matrix& res, size_t row_from, size_t row_to)
{
	for (size_t i = row_from; i < row_to; ++i) {
		for (size_t j = 0; j < lhs.cols_count(); ++j) {
			res.rows[i][j] -= rhs.rows[i][j];
		}
	}
}

template<typename T, typename _>
Matrix<T, _> Matrix<T, _>::operator*(const Matrix& rhs) const
{
	Matrix res(rows_count(), rhs.cols_count());
	if (cols_count() != rhs.rows_count()) {
		throw::std::invalid_argument("Incorrect dimension\n");
	}

	set_th_count(oper::mul, res.dim());
	parallel_op(mult, *this, rhs, res);
	return res;
}

template<typename T, typename _>
Matrix<T, _>& Matrix<T, _>::operator*=(const Matrix& rhs)
{
	Matrix res(rows_count(), rhs.cols_count());
	if (cols_count() != rhs.rows_count()) {
		throw::std::invalid_argument("Incorrect dimension\n");
	}

	set_th_count(oper::mul, res.dim());
	parallel_op(mult, *this, rhs, res);
	*this = res;

	return *this;
}

template <typename T, typename _>
void Matrix<T, _>::mult(const Matrix& lhs, const Matrix& rhs, Matrix& res, size_t from, size_t to)
{
	for (auto i = from; i < to; ++i) {
		for (size_t j = 0; j < rhs.cols_count(); ++j) {
			for (size_t k = 0; k < lhs.cols_count(); ++k) {
				res.rows[i][j] += lhs.rows[i][k] * rhs.rows[k][j];
			}
		}
	}
}


template <typename T, typename _>
void Matrix<T, _>::copy(const Matrix& m_from, Matrix<double>& m_to, size_t r_from, size_t r_to)
{
	for (size_t i = r_from; i < r_to; ++i) {
		for (size_t j = 0; j < m_to.cols_count(); ++j) {
			m_to.at(i, j) = static_cast<double>(m_from.at(i, j));
		}
	}
}

template <typename T, typename _>
void Matrix<T, _>::double_copy(const Matrix& m_from, Matrix<double>& m_to)
{
	std::vector<std::future<void>> threads;
	for (size_t i = 0; i < thCount - 1; ++i) {
		size_t from = i * m_to.rows_count() / thCount;
		size_t to = (i + 1) * m_to.rows_count() / thCount;

		threads.push_back(std::async(copy, std::ref(m_from), std::ref(m_to), from, to));
	}
	size_t from = (thCount - 1) * m_to.rows_count() / thCount;
	copy(m_from, m_to, from, m_to.rows_count());

	for (auto& i : threads) {
		i.get();
	}
}

template <typename T, typename _>
size_t Matrix<T, _>::max_pos(size_t col) const
{
	T max = std::abs(rows[col][col]);
	size_t max_pos = col;
	for (size_t i = col + 1; i < rows_count(); ++i) {
		T element = std::abs(rows[i][col]);
		if (element > max) {
			max = element;
			max_pos = i;
		}
	}
	return max_pos;
}

template <typename T, typename _>
double Matrix<T, _>::det() const
{
	if (rows_count() != cols_count() || rows_count() == 0) {
		throw std::invalid_argument("Invalid dimension\n");
	}
	if (rows_count() == 2) {
		return static_cast<double>(rows[0][0]) * static_cast<double>(rows[1][1])
			- static_cast<double>(rows[0][1]) * static_cast<double>(rows[1][0]);
	}
	double _det = 1;
	set_th_count(oper::det, dim());
	parallel_det(*this, _det);

	return _det;
}

template <typename T, typename _>
void Matrix<T, _>::triang(Matrix<double>& mtx, size_t col, size_t r_from, size_t r_to)
{
	for (size_t j = r_from; j < r_to; ++j) {
		double mult = static_cast<double>(-1) * mtx.at(j, col) / mtx.at(col, col);
		for (size_t k = col; k < mtx.rows_count(); ++k) {
			mtx.at(j, k) += mtx.at(col, k) * mult;
		}
	}
}

template <typename T, typename _>
void Matrix<T, _>::parallel_det(const Matrix& mtx, double& _det)
{
	Matrix<double> to_triang(mtx.rows_count(), mtx.cols_count());
	if constexpr (!std::is_same_v<T, double>) {
		double_copy(mtx, to_triang);
	}
	else {
		to_triang = mtx;
	}


	if (thCount == 7 && to_triang.rows_count() == 500) {
		std::cout << "";
	}
	int sgn = 1;
	for (size_t i = 0; i < to_triang.rows_count() - 1; ++i) {
		size_t max = to_triang.max_pos(i);
		if (std::abs(to_triang.at(max, i)) < std::numeric_limits<double>::epsilon()) {
			_det = 0;
			break;
		}
		if (i != max) {
			sgn *= -1;
			to_triang.swap(i, max);
		}

		std::vector<std::future<void>> threads;
		double n = static_cast<double>(to_triang.rows_count() - i - 1) / static_cast<double>(thCount);
		for (size_t j = 0; j < thCount; j++) {
			size_t from = static_cast<size_t>(std::round(j * n + 1 + i));
			size_t to = static_cast<size_t>(std::round((j + 1) * n + 1 + i));

			if (j < thCount - 1) {
				threads.push_back(std::async(triang, std::ref(to_triang), i, from, to));
			}
			else {
				triang(to_triang, i, from, to);
			}
		}
		for (auto& j : threads) {
			j.get();
		}
	}


	
	if (_det == 0) {
		return;
	}

	_det = 1;
	for (size_t i = 0; i < mtx.rows_count(); ++i) {
		_det *= to_triang.at(i, i);
	}
	_det *= sgn;
}
