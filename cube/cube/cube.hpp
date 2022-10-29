#ifndef __CUBE_H__
#define __CUBE_H__

#include <stdexcept>
#include <list>
#include <utility>	// std::pair

using color = enum color {
	yellow, blue,
	red, green,
	orange, white
};

using direction = enum direction {
	fwd,
	bcwd
};

using movement = enum movement {
	r, br, l, bl,			// ������, ������ � �������� �������, �����..
	f, bf, b, bb,
	u, bu, d, bd,
	hm, bhm
};

using step = enum step {
	Rand,
	Cross,
	First_Layer,
	Second_Layer,
	Second_Cross,
	Upper_Colors,
	Set_Upper_Corners,
	Rotate_Upper_Corners
};

using stepList = std::list<std::pair<movement, step>>;

class cube
{
public:
	virtual ~cube() {}; 
	
	virtual bool isSolved() const = 0;
	virtual bool isRubiks() const = 0;

	virtual void putPlate(size_t side, size_t position, color plateColor) = 0;
	virtual color getPlate(size_t side, size_t position) const = 0;
	virtual size_t dimension() const = 0;

private:

};

class tripleCube final : public cube
{
public:
	tripleCube();

	bool isSolved() const override;
	bool isRubiks() const override;
	size_t dimension() const override;

	void putPlate(size_t side, size_t position, color _color) override;
	color getPlate(size_t side, size_t position) const override;

	void doMovement(movement toDo, step curStep);

	void F(direction _direction, step curStep);		// �������� �����
	void R(direction _direction, step curStep);		// ������ �����
	void L(direction _direction, step curStep);		// ����� �����
	void B(direction _direction, step curStep);		// ������ �����
	void U(direction _direction, step curStep);		// ������� �����
	void D(direction _direction, step curStep);		// ������ �����
	void hM(direction _direction, step curStep);	// �������� �� �����������

	void resetSteps();
	const stepList& getSteps() const;
	stepList& getSteps();

private:
	color body[6][9];

	stepList steps;

	// ������ ������
	void SwapPlates(size_t firstSide, size_t firstPos,
		size_t secondSide, size_t secondPos);
	
	// ������� ����� �������
	void SideRotation(size_t side, direction _direction);	

	// ������ ������ fRowNum � RowNum ������ fSide � sSide
	void RowToRow(size_t fSide, size_t fRowNum, size_t sSide, size_t sRowNum);

	// ������ c������ fColNum � sColNum ������ fSide � sSide
	void ColToCol(size_t fSide, size_t fColNum, size_t sSide, size_t sColNum);

	// ������ ������ RowNum � ������� ColNum ������ fSide � sSide ��������������
	void RowToCol(size_t fSide, size_t RowNum, size_t sSide, size_t ColNum);
	


};

#endif // !__CUBE_H__