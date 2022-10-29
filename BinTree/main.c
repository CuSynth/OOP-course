//Пример использования
#include <string.h>
#include <assert.h>
#include <math.h>
#include "btree.h"
#include <stdlib.h>

static int int_compare(const void* lhsp, const void* rhsp) {
	const int* lhs = (const int*)lhsp;
	const int* rhs = (const int*)rhsp;

	return *rhs - *lhs;
}

static int gen_unique_key(int* keys, size_t size) {
	do {

		int r = (rand());
		bool ok = true;
		for (size_t j = 0; j < size && ok; ++j) {
			ok = r != keys[j];
		}
		if (ok) return r;
	} while (true);
}

#define testSize 100

static void insert_remove_test()
{
	void* btree = btree_create(sizeof(int), sizeof(int), int_compare);
	assert(0 == btree_count(btree));
	assert(btree_stop(btree) == btree_first(btree));

	int keys[testSize];
	srand(0);
	for (int i = 0; i < testSize; ++i) {
		keys[i] = gen_unique_key(keys, i);
	}
	size_t count = 0;
	for (size_t i = 0; i < testSize; ++i)
	{
		assert(btree_count(btree) == count);

 		bool create = false;
		int* a = (int*)btree_insert(btree, &keys[i], &create);
		if (!create)
		{
			assert(a == NULL);
			assert(btree_count(btree) == count);
			continue;
		}
		++count;
		assert(a != NULL);
		assert(btree_count(btree) == count);
		*a = (int)i;
	}

	assert(btree_count(btree) == count);

	for (size_t i = 0; i < testSize; ++i)
	{
		const bool exist = (btree_item(btree, &keys[i]) != NULL);
		assert(exist);
	}

	for (size_t i = 0; i < testSize; i += 2)
	{	
		const bool exist = (btree_item(btree, &keys[i]) != NULL);
		assert(exist);
		btree_remove(btree, &keys[i], NULL);
		count -= exist ? 1 : 0;

		assert(btree_count(btree) == count);
	}

	assert(btree_count(btree) == count);
}

int main(int argc, char* argv[])
{
	insert_remove_test();
	return 0;
}
