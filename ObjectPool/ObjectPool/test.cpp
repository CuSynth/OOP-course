#include "gtest/gtest.h"
#include "gtest-memleak.hpp"
#include "../ObjectPool/objectPool.hpp"

constexpr size_t test_size = 10000;
constexpr double epsilon = 1e-10;


using namespace std;

GTEST_API_ int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
#if defined(_MSC_VER)
	testing::UnitTest::GetInstance()->listeners().Append(new testing::MSVCMemoryLeakListener());
#endif // defined(_MSC_VER)
	return RUN_ALL_TESTS();
}

class custom {
public:
	int num;
	int* p_num{};

	//long long a = 10;	// big data, just for memLeak test
	//long long b = 10;
	//long long c = 10;
	//long long aa = 10;
	//long long ab = 10;
	//long long ac = 10;

	custom(int _a = 0) : num{ _a }, p_num{new int[test_size]} {
		/*cout << "Custom ctr: " << num << endl;*/
	}

	custom(custom& another) : num{ another.num }, p_num{new int[test_size]} {
		//cout << "Custom copy: " << num << endl;
	}

	~custom() { 
		delete p_num;
		/*cout << "Custom dtr: " << num << endl;*/ 
	}
};

TEST(ctr, intPool) {
	object_pool<int> pool(10);
}

TEST(ctr, customDataPool) {
	object_pool<custom> pool(10);
}

TEST(ctr, POD_obj) {
	struct P { int i; double d; };
	object_pool<P> pool(1);

	P& p = pool.alloc(1, 2.3);

	ASSERT_EQ(p.i, 1);
	ASSERT_NEAR(p.d, 2.3 + 1e-15, epsilon);

	ASSERT_NO_THROW(pool.free(p));
}

TEST(alloc, data) {
	object_pool<custom> pool(10);

	custom& a = pool.alloc(1);
	custom& b = pool.alloc(2);
	custom& c = pool.alloc(3);

	ASSERT_EQ(a.num, 1);
	ASSERT_EQ(b.num, 2);
	ASSERT_EQ(c.num, 3);
}

TEST(alloc, outOfRange) {
	object_pool<custom> pool(3);

	custom& a = pool.alloc(1);
	custom& b = pool.alloc(2);
	custom& c = pool.alloc(3);
	ASSERT_THROW(pool.alloc(4), out_of_range);
}

TEST(alloc, zeroSize) {
	object_pool<custom> pool(0);

	ASSERT_THROW(pool.alloc(0), out_of_range);
}

TEST(alloc, changeData) {
	object_pool<custom> pool(10);

	custom& a = pool.alloc(1);
	custom& b = pool.alloc(2);
	custom& c = pool.alloc(3);

	ASSERT_EQ(a.num, 1);
	ASSERT_EQ(b.num, 2);
	ASSERT_EQ(c.num, 3);

	a.num = 10;
	b.num = 20;
	c.num = 30;

	ASSERT_EQ(a.num, 10);
	ASSERT_EQ(b.num, 20);
	ASSERT_EQ(c.num, 30);
}

TEST(_free, goodData) {
	object_pool<custom> pool(3);

	custom& a = pool.alloc(1);
	custom& b = pool.alloc(2);
	custom& c = pool.alloc(3);

	pool.free(a);
	pool.free(b);
	pool.free(c);

	ASSERT_NO_THROW(a = pool.alloc(10));
	ASSERT_NO_THROW(b = pool.alloc(20));
	ASSERT_NO_THROW(c = pool.alloc(30));

	ASSERT_EQ(a.num, 10);
	ASSERT_EQ(b.num, 20);
	ASSERT_EQ(c.num, 30);
}

TEST(_free, badData) {
	object_pool<custom> pool(3);
	object_pool<custom> anotherPool(3);

	custom& a = pool.alloc(1);
	custom& notA = anotherPool.alloc(1);

	ASSERT_THROW(pool.free(notA), invalid_argument);
	ASSERT_THROW(anotherPool.free(a), invalid_argument);
}

TEST(_free, daletedTwice) {
	object_pool<custom> pool(3);

	custom& a = pool.alloc(1);

	ASSERT_NO_THROW(pool.free(a));
	ASSERT_THROW(pool.free(a), invalid_argument);
}

TEST(bigData, memLeakTest) {
	object_pool<custom> pool(test_size);
	for (int i = 0; i < test_size; ++i) {
		pool.free(pool.alloc(i));
	}

	for (int i = 0; i < test_size; ++i) {
		custom& tmp = pool.alloc(i);
		tmp.num = 10 * i;
	}

	//for (int i = 0; i < test_size/2; ++i) {
	//	custom tmp = pool.alloc(i);	// call copy ctr
	//	tmp.num = 10 * i;
	//}
}
