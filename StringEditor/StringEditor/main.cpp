#include <iostream>
#include <fstream>

#include "StrEditor.hpp"

using namespace std;

int main() {
	ifstream cmd_ifs("commands.txt");
	ifstream data_ifs("data.txt");
	
	StrEditor stredit(data_ifs, cmd_ifs, cout);
	stredit.edit();
	stredit.write(cout);

	return 0;
}