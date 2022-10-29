#include <iostream>

#include "bg_crop.hpp"

using namespace std;

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "Invalid argument." << endl << "Use: bg_crop inputFile.bmp outputFile.bmp" << endl;
		return -1;
	}

	string ifname(argv[1]);
	string ofname(argv[2]);

	cout << "Loading image \"" << ifname << "\".. ";
	bmp::Bitmap img;
	img.Load(ifname);
	cout << "Image loaded." << endl;

	cout << "Cropping.. ";
	Croper crp;
	crp.crop(img);
	cout << "Image cropped." << endl;

	cout << "Saving image to \"" << ofname << "\".. ";
	crp.GetNewBitmap().Save(ofname);
	cout << "Image saved." << endl;

	return 0;
}