#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "console.h"
#include "solver.hpp"

const size_t TEXT_COLOR_NUM = 8;

const char BLOCK_CHAR = ' ';
const char FRAME_CHAR = '#';

const int BACK_TEXT_PADDING = 5;
const size_t CUBE_PADDING = 3;	// Сдвиг по горизонтали
const size_t CUBE_OFFSET = 15;	// Сдвиг от верхушки печатаемого куба до текста

const size_t RND_STEP_COUNT = 15;



class interfacedReader
{
public:
	interfacedReader(tripleCube& _cube);
	bool chooseAndRead();

private:
	tripleCube& toWrite;

};

class solutionPrinter // Печатаем последовательность действий, применяемую к кубу
{
public:
	solutionPrinter(const stepList& _steps, tripleCube& _toPrint);
	bool print(bool isSolution);

private:
	const stepList& steps;
	tripleCube& toPrint;

	const char* getStepName(step _step) const;
	const char* getMovName(movement _movement) const;

	int maxX, maxY;
};

class randomizer
{
public:
	randomizer(tripleCube& _cube);
	void randomize();

private:
	tripleCube& toDestroy;
};



class interfacer {
public:
	interfacer();

	bool interrupt(int key);
	void printMenu();

	void rndProcess();
	void solverProcess();
	void demoProcess();
	void helpProcess();

private:
	int maxX, maxY;

	void initColors();
	void clearAndGo(size_t posX, size_t posY);


	void waitAnyKey();
};

#endif // !__INTERFACE_H__