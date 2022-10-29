#include "printer.hpp"

simpleConsolePrinter::simpleConsolePrinter() {
	con_getMaxXY(&maxX, &maxY);
}

void simpleConsolePrinter::charAt(char c, short col, size_t posX, size_t posY) {
 	if (posX > (size_t)maxX)
		throw std::invalid_argument("Wrong coords while placing cube char!\n");

	con_gotoXY(posX, posY);
	con_setColor(col);
	con_outTxt("%c", c);
}

void simpleConsolePrinter::PrintFrame(const cube& _cube, size_t posX, size_t posY, char c, short foregroundColor, short backgroundColor) {
	con_initPair(7, foregroundColor, backgroundColor);
	size_t dim = _cube.dimension();
	for (size_t i = dim+1; i < 2*dim+3; i++) {
		charAt(c, 7, posX + i, posY);
		charAt(c, 7, posX + i, posY + 3*dim+3);

		charAt(c, 7, posX, posY + i);
		charAt(c, 7, posX + 3*dim+3, posY + i);
		charAt(c, 7, posX + 4*dim+4, posY + i);
	}

	for (size_t i = 0; i < 4*dim+5; i++) {
		charAt(c, 7, posX + i, posY + dim+1);
		charAt(c, 7, posX + i, posY + 2*dim+2);
	}

	for (size_t i = 0; i < 3*dim+4; i++) {
		charAt(c, 7, posX + dim+1, posY + i);
		charAt(c, 7, posX + 2*dim+2, posY + i);
	}
}

void simpleConsolePrinter::PrintSide(const cube& _cubre, size_t sideNum, size_t posX, size_t posY, char c) {
	size_t dim = _cubre.dimension();
	for (size_t j = 0; j < dim; j++)
		for (size_t i = 0; i < dim; i++)
			charAt(c, _cubre.getPlate(sideNum, dim * j + i), posX + i, posY + j);
}

void simpleConsolePrinter::printCube(const cube& _cube, size_t posX, size_t posY, char c) {
	size_t dim = _cube.dimension();
	PrintSide(_cube, 0, posX + dim+2, posY + 1, c);
	PrintSide(_cube, 1, posX + 1, posY + dim+2, c);
	PrintSide(_cube, 2, posX + dim+2, posY + dim+2, c);
	PrintSide(_cube, 3, posX + 2*dim+3, posY + dim+2, c);
	PrintSide(_cube, 4, posX + 3*dim+4, posY + dim+2, c);
	PrintSide(_cube, 5, posX + dim+2, posY + 2*dim+3, c);
}