#include <iostream>
#include <fstream>

#include "parser.hpp"
#include "printer.hpp"

using namespace std;

int main(int argc, char** argv) {
	if (argc != 4) {
		cout << "Invalid argument." << endl << "Use: inputFile.txt outputFile.txt max_people_count" << endl;
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

	size_t maxCount = atoi(argv[3]);


	try {

		cout << "Parsing.." << endl;
		Parser ps(ifs);
		PlacesDB db;
		ps.parse(db);
		auto dat = db.getPlaces();

		cout << "Calculating.." << endl;
		Calculator calc(db, maxCount);
		calc.Calculate();

		cout << "Printing.." << endl;
		Printer inf(cout);
		inf.print(calc.GetCrowds());
		cout << "Done!" << endl;


	}
	catch (const std::invalid_argument err) {
		cout << err.what() << endl;
	}

	ifs.close();
	ofs.close();
	return 0;
}