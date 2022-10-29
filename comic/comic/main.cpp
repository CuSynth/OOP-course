#include <iostream>
#include "comics.hpp"

int main(int argc, char** argv) {
	std::string fname;
	if (argc == 1) {
		fname = "sample.jpg";
	}
	else {
		fname = argv[1];
	}

	try {
		comics comic(fname);
		comic.process();
	}
	catch (const std::exception& e) {
		std::cout << "Error while processing: " << e.what();
	}

	return 0;
}