#include "reader.hpp"

#include "console.h"


color reader::getColByChar(char ch) {
	switch (ch)
	{
	case 'y':
	case 'Y':
		return yellow;

	case 'b':
	case 'B':
		return blue;

	case 'r':
	case 'R':
		return red;

	case 'g':
	case 'G':
		return green;

	case 'o':
	case 'O':
		return orange;

	case 'w':
	case 'W':
		return white;

	default:
		throw std::invalid_argument("Wrong color while reading! ");
	}
}

void rowReader::read(cube& _cube) {
	std::string line;
	char C;
	size_t side = 0;
	size_t dimension = _cube.dimension();
	size_t plateCount = dimension * dimension;

	while (getline(is, line)) {
		std::istringstream iss(line);
		for (size_t i = 0; i < plateCount; ++i) {
			if (!(iss >> C))
				throw std::invalid_argument("Wrong row format!\n");
			color col = getColByChar(C);
			_cube.putPlate(side, i, col);
		}

		if (++side == 6)
			break;
	}
}


anotherReader::anotherReader(size_t _posX, size_t _posY) : posX(_posX), posY(_posY) {}

void anotherReader::read(cube& cb) {
	con_clearScr();
	prtr.PrintFrame(cb, posX, posY, '#', CON_COLOR_WHITE, CON_COLOR_BLACK);

	size_t dim = cb.dimension();
	int curX = posX + dim + 2;
	int curY = posY + 1;
	int key;


	while (true) {
		prtr.printCube(cb, posX, posY, ' ');

		con_gotoXY(curX, curY);
		con_showCursor();
		while (!con_keyPressed());
		key = con_getKey();
		con_hideCursor();

		if (key == CON_KEY_ESCAPE || key == CON_KEY_ENTER)
			break;

		switch (key)
		{
		case(CON_KEY_LEFT):
			if(checkBorders(--curX, curY, dim))
				++curX;			
			continue;
		case(CON_KEY_RIGHT):
			if (checkBorders(++curX, curY, dim))
				--curX;
			continue;
		case(CON_KEY_UP):
			if (checkBorders(curX, --curY, dim))
				++curY;
			continue;
		case(CON_KEY_DOWN):
			if (checkBorders(curX, ++curY, dim))
				--curY;
			continue;
		default:
			break;
		}



		size_t sd = sideNumFromXY(curX, curY, dim);
		switch (sd)
		{
		case 0:
			cb.putPlate(0, posFromXY(curX - dim - 2 - posX, curY - 1 - posY, dim), getColByChar(key));
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			cb.putPlate(sd, posFromXY(curX - 1 - (sd - 1) * (dim + 1) - posX, curY - dim - 2 - posY, dim), getColByChar(key));
			break;
		case 5:
			cb.putPlate(5, posFromXY(curX - dim - 2 - posX, curY - 2 * dim - 3 - posY, dim), getColByChar(key));
			break;
		default:
			break;
		}
	}
}

bool anotherReader::checkBorders(size_t x, size_t y, size_t dim) {
	x -= posX;
	y -= posY;

	if (y == 0 || y == 3 * dim + 3 ||
		x == 0 || x == 4 * dim + 4)
		return true;

	if ((y <= dim + 1 || y >= 2 * (dim + 1)) && (x >= 2 * (dim + 1) || x <= dim + 1))
		return true;
	if ((x == dim + dim / 2 + 2 && y == dim / 2 + 1) ||
		(x == dim + dim / 2 + 2 && y == dim / 2 + 2 * dim + 3))
		return true;

	for (size_t i = 0; i < 4; i++) {
		if (x == dim / 2 + 1 + i * (dim + 1) && y == dim / 2 + dim + 2)
			return true;
	}

	return false;
}

size_t anotherReader::sideNumFromXY(size_t x, size_t y, size_t dim) {
	x -= posX;
	y -= posY;

	if (y < dim + 1)
		return 0;
	if (y > 2 * (dim + 1))
		return 5;
	if (y == dim + 1 || y == 2 * dim + 2)
		return -1;

	if (x < dim + 1)
		return 1;
	if (x < 2 * (dim + 1) && x > dim + 1)
		return 2;
	if (x < 3 * (dim + 1) && x > 2 * (dim + 1))
		return 3;
	if (x < 4 * (dim + 1) && x > 3 * (dim + 1))
		return 4;
	return -1;
}

size_t anotherReader::posFromXY(size_t x, size_t y, size_t dim) {
	return dim * y + x;
}