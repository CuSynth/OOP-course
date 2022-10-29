#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "cube.hpp"
#include "console.h"


class printer
{
public:
	virtual ~printer() {};

	virtual void PrintFrame(const cube& _cube, size_t posX, size_t posY, char c, short foregroundCol, short backgroundCol) = 0;
	virtual void PrintSide(const cube& _cube, size_t side, size_t posX, size_t posY, char c) = 0;
	virtual void printCube(const cube& _cube, size_t posX, size_t posY, char c) = 0;

private:

};


class simpleConsolePrinter final : public printer
{
public:
	simpleConsolePrinter();

	void charAt(char c, short col, size_t x, size_t y);

	void PrintFrame(const cube& _cube, size_t posX, size_t posY, char c, short foregroundCol, short backgroundCol) override;
	void PrintSide(const cube& _cube, size_t side, size_t posX, size_t posY, char c) override;
	void printCube(const cube& _cube, size_t posX, size_t posY, char c) override;

private:
	int maxX, maxY;

};


//class anotherPrinter : public printer
//{
//public:
//	virtual void PrintFrame(const Cube& cb, size_t posX, size_t posY, char c, short col_f, short col_b) {};
//	virtual void PrintSide(const Cube& cb, size_t numside, size_t posX, size_t posY, char c) {};
//	virtual void printCube(const Cube& cb, size_t posX, size_t posY, char c) {};
//
//private:
//
//};

#endif // !__PRINTER_H__