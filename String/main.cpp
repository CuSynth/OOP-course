#include <iostream>
#include "String.hpp"

int main() {

	String hello("Hello");
	String sp(1, ' ');
	String world("world!");

	hello += sp;
	hello += world;

	printf("%d, %d, _%s_\n", (int)hello.size(), (int)hello.capacity(), hello.data());
	return 0;
}