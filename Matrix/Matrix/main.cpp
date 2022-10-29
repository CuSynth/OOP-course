#include <iostream>
#include <array>
#include <iomanip>
#include <chrono>

#include "matrix.hpp"

using namespace std;
using namespace std::chrono;


constexpr size_t theads_max = 9u;

const auto dimensions = { 6, 7, 10, 11, 15, 25, 50, 111, 150, 201, 300, 500, 501, 1000 };
const auto det_dim = dimensions;
const auto mult_dim = dimensions;
const auto sum_dim = { 6, 7, 10, 11, 15, 25, 50, 111, 150, 201, 300, 500, 501, 1000, 5000, 1000};



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

template<size_t Threads>
void PrintResult(std::ostream& stream, const std::initializer_list<int>& dimensions, std::array<std::vector<double>, Threads>& times)
{
    stream << std::setw(6) << "dim/th" << ";";
    for (auto dim : dimensions) {
        stream << std::setw(12) << dim << ";";
    }
    stream << std::endl;
    for (size_t th = 0; th < times.size(); ++th) {
        stream << std::setw(6) << (th + 1) << ";";
        for (size_t dim = 0; dim < times[th].size(); ++dim) {
            stream << std::setw(12) << std::fixed << times[th][dim] << ";"; stream.flush();
        }
        stream << std::endl;
    }
}

void DeterminantTest(std::ostream& stream) {
    stream << "Determinant:" << std::endl;
    std::array<std::vector<double>, theads_max> times;

    stream << std::setw(11) << "th/dim" << ";";
    for (size_t th = 1; th <= theads_max; ++th) {
        stream << std::setw(12) << th << ";";
    }
    stream << std::endl;
    stream << std::setprecision(7);

    for (auto dim : det_dim) {
        auto d = diagonal<double>(dim, 0.0, 1.0);
        double res = ((double)(dim & 1) != 0 ? 1 : -1) * ((double)dim - 1);
        
        stream << std::setw(4) << d.cols_count();
        stream << "(" << std::setw(5) << std::defaultfloat << res << ");"; stream.flush();
        

        for (size_t th = 1; th <= theads_max; ++th) {
            double time = std::numeric_limits<double>::max();
            d.set_threads(th);
            for (int i = 0; i < 5; i++)
            {
                if (th == 7 && dim == 500) {
                    cout << "";
                }


                auto start = high_resolution_clock::now();
                auto det = d.det();
                time = std::min(time, duration<double>(high_resolution_clock::now() - start).count());
                if (std::abs(res - det) > 1e-8) {
                    stream << std::endl << "ERROR: calculation(" << dim << "/" << th << ") current det = " << std::setprecision(11) << std::fixed << det << " != res = " << res; stream.flush();
                    break;
                }
            }
            stream << std::setw(12) << std::setprecision(7) << std::fixed << time << ";"; stream.flush();
            times[th - 1].emplace_back(time);
        }
        stream << std::endl;
    }

    std::cout << "\nDet test res:" << std::endl;
    PrintResult(stream, det_dim, times);
    std::cout << "Det test finished\n" << std::endl;
}

void MultTest(std::ostream& stream) {
    stream << "Mult:" << std::endl;
    std::array<std::vector<double>, theads_max> times;

    stream << std::setw(11) << "th/dim" << ";";
    for (size_t th = 1; th <= theads_max; ++th) {
        stream << std::setw(12) << th << ";";
    }
    stream << std::endl;
    stream << std::setprecision(7);

    for (auto dim : mult_dim) {
        auto d1 = diagonal<double>(dim, 0.0, 1.0);
        auto d2 = diagonal<double>(dim, 0.0, 1.0);
        auto exp = diagonal<double>(dim, (double)(dim - 1), (double)(dim - 2));

        stream << std::setw(9) << exp.cols_count();

        for (size_t th = 1; th <= theads_max; ++th) {
            double time = std::numeric_limits<double>::max();
            exp.set_threads(th);
            for (int i = 0; i < 5; i++)
            {
                auto start = high_resolution_clock::now();
                auto res = d1 * d2;
                time = std::min(time, duration<double>(high_resolution_clock::now() - start).count());
                if (res != exp) {
                    stream << std::endl << "ERROR: calculation"; stream.flush();
                    break;
                }
            }
            stream << std::setw(12) << std::setprecision(7) << std::fixed << time << ";"; stream.flush();
            times[th - 1].emplace_back(time);
        }
        stream << std::endl;
    }

    std::cout << "\nMult test res:" << std::endl;
    PrintResult(stream, mult_dim, times);
    std::cout << "Mult test finished\n" << std::endl;
}

void SumTest(std::ostream& stream) {
    stream << "Sum:" << std::endl;
    std::array<std::vector<double>, theads_max> times;

    stream << std::setw(11) << "th/dim" << ";";
    for (size_t th = 1; th <= theads_max; ++th) {
        stream << std::setw(12) << th << ";";
    }
    stream << std::endl;
    stream << std::setprecision(7);

    for (auto dim : sum_dim) {
        auto d1 = diagonal<double>(dim, 0.0, 1.0);
        auto d2 = diagonal<double>(dim, 0.0, 2.0);
        auto exp = diagonal<double>(dim, 0.0, 3.0);

        stream << std::setw(9) << exp.cols_count();

        for (size_t th = 1; th <= theads_max; ++th) {
            double time = std::numeric_limits<double>::max();
            exp.set_threads(th);
            for (int i = 0; i < 5; i++)
            {
                auto start = high_resolution_clock::now();
                auto res = d1 + d2;
                time = std::min(time, duration<double>(high_resolution_clock::now() - start).count());
                if (res != exp) {
                    stream << std::endl << "ERROR: calculation"; stream.flush();
                    break;
                }
            }
            stream << std::setw(12) << std::setprecision(7) << std::fixed << time << ";"; stream.flush();
            times[th - 1].emplace_back(time);
        }
        stream << std::endl;
    }
    std::cout << "\nSum test res:" << std::endl;
    PrintResult(stream, sum_dim, times);
    std::cout << "Sum test finished\n" << std::endl;
}

void SubstrTest(std::ostream& stream) {
    stream << "Substr:" << std::endl;
    std::array<std::vector<double>, theads_max> times;

    stream << std::setw(11) << "th/dim" << ";";
    for (size_t th = 1; th <= theads_max; ++th) {
        stream << std::setw(12) << th << ";";
    }
    stream << std::endl;
    stream << std::setprecision(7);

    for (auto dim : sum_dim) {
        auto d1 = diagonal<double>(dim, 0.0, 1.0);
        auto d2 = diagonal<double>(dim, 0.0, 2.0);
        auto exp = diagonal<double>(dim, 0.0, -1.0);

        stream << std::setw(9) << exp.cols_count();

        for (size_t th = 1; th <= theads_max; ++th) {
            double time = std::numeric_limits<double>::max();
            exp.set_threads(th);
            for (int i = 0; i < 5; i++)
            {
                auto start = high_resolution_clock::now();
                auto res = d1 - d2;
                time = std::min(time, duration<double>(high_resolution_clock::now() - start).count());
                if (res != exp) {
                    stream << std::endl << "ERROR: calculation"; stream.flush();
                    break;
                }
            }
            stream << std::setw(12) << std::setprecision(7) << std::fixed << time << ";"; stream.flush();
            times[th - 1].emplace_back(time);
        }
        stream << std::endl;
    }

    std::cout << "\nSubstr test res:" << std::endl;
    PrintResult(stream, sum_dim, times);
    std::cout << "Substr test finished\n" << std::endl;
}


int main() {
    std::ostream& st = std::cout;
    DeterminantTest(st);
    MultTest(st);
    SumTest(st);
    SubstrTest(st);

    return 0;
}