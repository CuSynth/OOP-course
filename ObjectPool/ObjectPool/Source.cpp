#include <iostream>
#include "objectPool.hpp"

using namespace std;

class custom {
public:
	int num;
	long long a = 10;
	long long b = 10;
	long long c = 10;
	long long aa = 10;
	long long ab = 10;
	long long ac = 10;

	custom(int _a) : num(_a) {
		cout << "Tmp ctr: " << num << endl;
	}

	custom(custom& another) {
		num = another.num;
		cout << "COPY!!!\n";
	}


	~custom() {
		cout << "Tmp dtr: " << num << endl;
	}
};

int main() {
	

	cout << sizeof(char) << ' ' << sizeof(char*) << ' ' << sizeof(custom) << endl;

	object_pool<custom> pool(3);
	object_pool<custom> n_pool(5);
	custom& a1 = pool.alloc(1); 
	custom& a2 = pool.alloc(2);
	custom& a3 = pool.alloc(3);

	custom& _a1 = n_pool.alloc(1);


	cout << a1.num << ' ' << a2.num << ' ' << a3.num << endl;


	a1.num = 10;
	a2.num = 20;
	a3.num = 30;

	cout << a1.num << ' ' << a2.num << ' ' << a3.num << endl;

	pool.free(a1);
	pool.free(a3);

	a1 = pool.alloc(100);
	a3 = pool.alloc(300);
	a2.num = 200;

	cout << a1.num << ' ' << a2.num << ' ' << a3.num << endl;

	//custom& a4 = pool.alloc(4);
	//custom& a5 = pool.alloc(5);

	cout << "Hw\n";

	return 0;
}