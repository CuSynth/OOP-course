#include <iostream>
#include "skipList.hpp"

using namespace std;

int main() {

	std::cout << "\nFirst list ctr:" << '\n';
	skip_list<int, int> lst;
	std::cout << "\nFirst list insert:" << '\n';
	lst[1] = 2;
	lst[2] = 3;
	lst[3] = 4;

	std::cout << "\nSecond list ctr:" << '\n';
	auto nlst = skip_list<int, int>(lst);

	std::cout << "\nTest:" << '\n';
	cout << nlst.empty() << '\n';
	cout << nlst.size() << '\n';
	cout << (lst.begin() != lst.end()) << '\n';
	cout << nlst[1] << '\n';

	return 0;
} 