#include <iostream>
#include <fstream>

#include "sTree.hpp"
#include "structs.hpp"
#include "IO.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	const char* inFile = "input.txt";
	const char* outFile = "out.txt";

	if (argc == 1) {
		cout << "Using default files." << endl;
	}
	else if (argc == 3) {
		inFile = argv[1];
		outFile = argv[2];
	}
	else {
		cerr << "Wrong param count! Use: input_file_name output_file_name\n";
		return -1;
	}

	cout << "Reading file " << inFile << "." << endl;
	ifstream inp(inFile);
	if (!inp) {
		cerr << "There is no file \"" << inFile << "\"!" << endl;
		return -1;
	}

	vertex_vector vertexes = reader(inp);
	MinSpanningTree tree(vertexes);
	mst newMst = tree.Kruskal();

	cout << "Writing to file " << outFile << ".." << endl;
	ofstream out(outFile);
	if (!out) {
	cerr << "There is no file \"" << outFile << "\"!" << endl;
		return -1;
	}
	
	out << newMst;


	out.close();
	inp.close();

	return 0;
}