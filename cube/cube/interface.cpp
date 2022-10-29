#include "interface.hpp"

#include "reader.hpp"
#include "printer.hpp"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

const char* ANY_KEY = "Press any key to go back.";

interfacedReader::interfacedReader(tripleCube& _cube) : toWrite(_cube) {}

bool interfacedReader::chooseAndRead() {
	std::cout << "Do you want to read the cube from file, from console or by interactive way (f/c/i)? Esc to go back." << std::endl;

	try
	{
		while (true) {
			if (!con_keyPressed())
				continue;
			int key = con_getKey();

			if (key == CON_KEY_C) {
				std::cout << "Enter your cube:" << std::endl;
				rowReader _reader(std::cin);
				_reader.read(toWrite);

				return true;
			}
			else if (key == CON_KEY_F) {
				std::cout << "Enter file name: ";
				std::string fname;
				std::cin >> fname;

				std::ifstream ifs(fname);
				if (!ifs) {
					std::cout << "Wrong filename! " << ANY_KEY << std::endl;
					while (!con_keyPressed());
					key = con_getKey();
					return false;
				}

				rowReader _reader(ifs);
				_reader.read(toWrite);

				return true;
			}
			else if (key == CON_KEY_I) {
				std::cout << "Enter your cube:" << std::endl;
				anotherReader _reader(CUBE_PADDING, 2);
				_reader.read(toWrite);
				con_clearScr();
				con_gotoXY(0, 0);
				con_setColor(TEXT_COLOR_NUM);

				return true;
			}
			else if (key == CON_KEY_ESCAPE)
				return false;

		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << ANY_KEY << std::endl;
		while (!con_keyPressed());
		con_getKey();
		return false;
	}
}


solutionPrinter::solutionPrinter(const stepList& _steps, tripleCube& _toPrint) :
	steps(_steps), toPrint(_toPrint) 
{
	con_getMaxXY(&maxX, &maxY);
}

const char* solutionPrinter::getStepName(step _step) const {
	const char* steps[] = { "Randomize", "Cross", "First layer", "Second layer",
				"Second cross", "Upper colors", "Setting corners", "Rotating corners" };
	return steps[(size_t)_step];
}

const char* solutionPrinter::getMovName(movement _movement) const {
	const char* movs[] = { "R", "R'", "L", "L'", "F", "F'", "B", "B'",
										"U", "U'", "D", "D'", "hM", "hM'" };
	return movs[(size_t)_movement];
}

bool solutionPrinter::print(bool isSolution) {	// Печатаем куб и решение
	std::cout << "Your cube now is:" << std::endl;

	int posX, posY;
	con_getXY(&posX, &posY);

	int cubeX = posX;
	int cubeY = posY;

	posY += CUBE_OFFSET;

	simpleConsolePrinter _printer;
	_printer.PrintFrame(toPrint, (size_t)(cubeX) + CUBE_PADDING, (size_t)cubeY + 1, FRAME_CHAR, CON_COLOR_WHITE, CON_COLOR_BLACK);
	_printer.printCube(toPrint, (size_t)(cubeX) + CUBE_PADDING, (size_t)cubeY + 1, BLOCK_CHAR);

	con_gotoXY(posX, posY);
	con_setColor(TEXT_COLOR_NUM);
	std::cout << (isSolution ? "Use this to solve it. Any key for next step, esc for quit.\n" : "Try this");

	step previousStep = Rand;
	for (auto it = steps.begin(); it != steps.end(); it++) {
		if (previousStep != it->second) {
			while (!con_keyPressed()) {}
			if(con_getKey() == CON_KEY_ESCAPE) 
				return false;

			if (previousStep != Rand)
				std::cout << std::endl << std::endl;

			previousStep = it->second;
			con_setColor(TEXT_COLOR_NUM);
			toPrint.doMovement(it->first, it->second);
			std::cout << getStepName(it->second) << ": " << getMovName((it++)->first);
		}
		
		std::cout << " -> " << getMovName(it->first);
		toPrint.doMovement(it->first, it->second);

		con_getXY(&posX, &posY);
		if (posX + BACK_TEXT_PADDING >= maxX) {
			posX = 0;
			++posY;
		}

		_printer.printCube(toPrint, (size_t)(cubeX)+CUBE_PADDING, (size_t)cubeY + 1, BLOCK_CHAR);
		con_gotoXY(posX, posY);
		con_setColor(TEXT_COLOR_NUM);
	}

	if(isSolution)
		std::cout << "\nCube is solved! " << ANY_KEY << std::endl;
	return true;
}


randomizer::randomizer(tripleCube& _cube) : toDestroy(_cube) {}

void randomizer::randomize() {
	srand((int)time(NULL));

	for (size_t i = 0; i < RND_STEP_COUNT; i++) {
		movement mov = (movement)(rand() % 12);
		toDestroy.doMovement(mov, Rand);
	}
}


interfacer::interfacer() {
	con_init();
	initColors();
	con_hideCursor();
	con_getMaxXY(&maxX, &maxY);
}

void interfacer::initColors() {
	con_initPair(yellow, CON_COLOR_BLACK, CON_COLOR_YELLOW);
	con_initPair(blue, CON_COLOR_BLACK, CON_COLOR_BLUE);
	con_initPair(red, CON_COLOR_BLACK, CON_COLOR_RED);
	con_initPair(green, CON_COLOR_BLACK, CON_COLOR_GREEN);
	con_initPair(orange, CON_COLOR_BLACK, CON_COLOR_MAGENTA);
	con_initPair(white, CON_COLOR_BLACK, CON_COLOR_WHITE);
	con_initPair(TEXT_COLOR_NUM, CON_COLOR_WHITE, CON_COLOR_BLACK);
}

void interfacer::clearAndGo(size_t posX, size_t posY) {
	if (posX > (size_t)maxX)
		throw std::invalid_argument( "Wrong coords while placing text position!\n" );

	con_clearScr();
	con_gotoXY(posX, posY);
	con_setColor(TEXT_COLOR_NUM);
}

bool interfacer::interrupt(int key) {
	switch (key) {
	case(CON_KEY_ONE):
	//case(CON_KEY_ONE_NUM):
		rndProcess();
		break;
	case(CON_KEY_TWO):
	//case(CON_KEY_TWO_NUM):
		solverProcess();
		break;
	case(CON_KEY_THREE):
	//case(CON_KEY_THREE_NUM):
		demoProcess();
		break;
	case(CON_KEY_ZERO):
	//case(CON_KEY_ZERO_NUM):
		helpProcess();
		break;

	case(CON_KEY_ESCAPE):
		clearAndGo(0, 0);
		return true;
	default:
		break;
	}

	printMenu();
	return false;
}

void interfacer::printMenu() {
	clearAndGo(0, 0);
	std::cout << "Menu. Use buttons 1-3 to do smthng." << std::endl
		<< "   1) Get random cube." << std::endl
		<< "   2) Solve your cube." << std::endl
		<< "   3) Demo mode." << std::endl
		<< "   0) Help." << std::endl
		<< "   Esc to exit." << std::endl;
}

void interfacer::waitAnyKey() {
	while (!con_keyPressed()) {}
	con_getKey();
}



void interfacer::rndProcess() {
	clearAndGo(0, 0);
	
	tripleCube toDestroy;
	tripleCube toPrint;

	randomizer rnd(toDestroy);	// Разбираем один
	rnd.randomize();

	solutionPrinter prtr(toDestroy.getSteps(), toPrint);	// На примере второго показываем, как разбирать
	prtr.print(false);

	std::cout << std::endl << ANY_KEY;
	waitAnyKey();
}

void interfacer::solverProcess() {
	clearAndGo(0, 0);

	tripleCube toSolve;
	interfacedReader iReader(toSolve);

	if (!iReader.chooseAndRead()) {
		return;
	}
	if (!toSolve.isRubiks()) {
		std::cout << "Wrong Cube data!\n" << ANY_KEY << std::endl;
		waitAnyKey();
		return;
	}
		
	tripleCube toPrint(toSolve);	
	beginnerSolver slvr(toSolve);
	clearAndGo(0, 0);

	if (!slvr.solve()) {
		std::cout << "Ooops.. Your cube have no solution! " << ANY_KEY << std::endl;
	}
	else {
		solutionPrinter prtr(slvr.getsolution(), toPrint);
		if(!prtr.print(true))
			return;
	}

	waitAnyKey();
}

void interfacer::demoProcess() {
	clearAndGo(0, 0); 

	tripleCube toDestroy;
	tripleCube toPrint;

	int posX = 0;
	int posY = 0;
	
	randomizer rnd(toDestroy);
	rnd.randomize();

	solutionPrinter prtr(toDestroy.getSteps(), toPrint);	// Рисуем и разбираем
	prtr.print(false);

	beginnerSolver solver(toDestroy);
	solver.solve();

	con_getXY(&posX, &posY);
	posX = 0;
	++posY;
	con_gotoXY(posX, posY);
	con_setColor(TEXT_COLOR_NUM);
	
	solutionPrinter anotherPrtr(toDestroy.getSteps(), toPrint);	// Рисуем и собираем
	if (!prtr.print(true))
		return;

	waitAnyKey();
}

void interfacer::helpProcess() {
	clearAndGo(0, 0);
	std::cout << "In file/console you have to enter your cube in order: y side, b, r, g, o, w." << std::endl
		<< "Cube have to be entered like in its development." << std::endl
		<< "Every side have to be printed in one row, from left-top block to right-bottom." << std::endl << std::endl;

	std::cout << "Use arrows to move in interactive mode. In every plate use buttons y, b, r, g, o and w to set colors." << std::endl << std::endl;

	std::cout << "Use no param to start full program." << std::endl
		<< "Use 'rand' param to get random cube, 'solve' to solve your cube.\nUse 'demo' to see demo mode, 'help' to see this menu" << std::endl;

	std::cout << std::endl << ANY_KEY << std::endl;

	waitAnyKey();
}