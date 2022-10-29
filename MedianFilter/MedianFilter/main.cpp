#include <iostream>
#include <fstream>

#include "IO.hpp"
#include "MedianFilter.hpp"

// -������ ������� - � ������ �������� ����
// -���������� ���������� ������ � ���������
// -� ������� ����� ������� ��� � ������ stride
// 

using namespace std;

int main(int argc, char* argv[])
{
	std::string dataFileName = "data.txt";

	if (argc == 1) {
		cout << "Using default data file." << endl;
	}
	else if (argc == 2) {
		dataFileName = argv[1];
	}
	else {
		cerr << "Wrong param!" << endl;
		return -1;
	}

	ifstream inp(dataFileName);
	if (!inp) {
		cerr << "There is no file \"" << dataFileName << "\"!" << endl;
		return -1;
	}

	RawData dat = reader(inp);

	bmp::Bitmap img;
	img.Load(dat.IFName);

	MedianFilter newImg;
	newImg.filter(img, dat.size, dat.level);
	img = newImg.get_result();

	img.Save(dat.OFName);
	inp.close();

	return 0;
}