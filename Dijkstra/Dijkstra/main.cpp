#include <iostream>
#include <fstream>
#include "Dijkstra.hpp"

using namespace std;

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "Invalid argument." << endl << "Use: inputFile.txt outputFile.txt" << endl;
		return -1;
	}
	
	ifstream ifs(argv[1]);
	if (!ifs) {
		cout << "Cant open file \"" << argv[1] << "\"." << endl;
		return -1;
	}

	ofstream ofs(argv[2]);
	if (!ofs) {
		cout << "Cant open file \"" << argv[2] << "\"." << endl;
		return -1;
	}

	Graph grp;
	Parser p(ifs);
	p.parse(grp);
	
	Dijkstra DAlg(grp);
	DAlg.calculate();
	
	DIter it(DAlg);
	Printer prt(ofs);
	prt.print(it);

	ofs.close();
	ifs.close();
	return 0;
}