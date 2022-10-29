#include <iostream>
#include <vector>
#include <deque>

#include "Radix.hpp"


int main() {

	std::deque<int> queue_data{ 3, 1, -10, 5, 2, -15, 10, 0, 1, -1 };
	long arr_data[] ={ 3, 1, -10, 5, 2, -15, 10, 0, 1, -1 };


	radixSort(queue_data.begin(), queue_data.end());
	radixSort(arr_data, arr_data + 10);

	std::cout << "queue: ";
	for (auto elem : queue_data) {
		std::cout << elem << ' ';
	}

	std::cout << "\narr: ";

	for (auto elem : arr_data) {
		std::cout << elem << ' ';
	}



	return 0;
}