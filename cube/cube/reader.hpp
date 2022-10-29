#ifndef __READER_H__
#define __READER_H__

#include "cube.hpp"
#include "printer.hpp"

#include <sstream>

class reader
{
public:
	virtual ~reader() {};
	virtual void read(cube& _cube) = 0;

	color getColByChar(char ch);
private:

};


class rowReader final : public reader
{
public:
	rowReader(std::istream& _is) : is(_is) {};
	void read(cube& _cube) override;

private:
	std::istream& is;

};


class anotherReader final : public reader
{
public:
	anotherReader(size_t _posX, size_t _posY);
	void read(cube& cb) override;

private:
	simpleConsolePrinter prtr;
	size_t posX, posY;

	bool checkBorders(size_t x, size_t y, size_t dim);
	size_t sideNumFromXY(size_t x, size_t y, size_t dim);
	size_t posFromXY(size_t x, size_t y, size_t dim);
};


#endif // !__READER_H__