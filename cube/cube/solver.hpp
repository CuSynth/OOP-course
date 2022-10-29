#ifndef __SOLVER_H__
#define __SOLVER_H__

#include "cube.hpp"

class tripleSolver
{
public:
	virtual ~tripleSolver() {};

	virtual	bool solve() = 0;
	virtual const stepList& getsolution() const = 0;

private:

};

class beginnerSolver final : public tripleSolver
{
public:
	beginnerSolver(tripleCube& _cb) : toSolve(_cb) {};

	bool solve() override;
	const stepList& getsolution() const override;

private:
	tripleCube& toSolve;

	void optimize();

	// �������� ��������
	movement GetReversed(movement mov);

	//
	// ����� ��������������� �������
	//

	void RURU(step stp);	// ���-���

	bool CheckColor(size_t sd, size_t pos, color col);	// ����� �� col �� ����� pos, �� ������� sd
	bool CheckTwoOnSide(size_t side, size_t f_pos, size_t s_pos);	// ��������� �� ����� �� ������� sd � ������ f_pos, s_pos

	// ������� ���� �� �� ������ ����� ����� ������ ����
	void UUntilPlateHaveColor(direction dir, step stp,
		size_t side, size_t pos, color col, bool cond);

	// �������, ���� �� ������ ������ �����
	void UUntilNotEdge(direction dir, step stp,
		size_t num, color f_col, color s_col);

	//
	// �����:
	//

	bool EdgeHaveColor(size_t num, color col);	// ���� �� col �� ����� num
	bool CheckEdge(size_t num, color f_col, color s_col);	// ���� �� f_col, s_col �� num

	void SolveCross();

	//
	// ������ ����:
	//

	bool CornerHaveColor(size_t num, color col);	// ���� �� col �� ���� num
	bool CheckCorner(size_t num, color f_col, color s_col, color t_col);	//	���� �� f_col, s_col, t_col �� ���� num
	
	// �������, ���� ����/��� ����� �� ������ �����
	void UUntilCornerHaveColor(direction dir, step stp,
		size_t num, color col, bool cond);


	void SolveFirstLayer();

	//
	// ������ ����
	//

	void UtoR(step stp);	// �������� ����� �������
	void UtoRr(step stp);	// �������� � ����������
	
	// �������, ���� ����/��� ����� �� ������ ������
	void UUntilEdgeHaveColor(direction dir, step stp,
		size_t num, color col, bool cond);

	void SolveSecondLayer();

	//
	// ������� �����
	//

	void AngleLineCross(step stp);	// ����->�����->�����->����.. �� ������� �������
	void SolveUpperCross();		// �������� �����
	bool CheckUpperColors();	// �� ������ �� ��� ������� �����
	void PlaceUpperColors();	// ������ ������� ����������� � ����� �����
	
	void SetUpperColors();		// ������ �� ������ ������� ����� ������

	//
	// ������ ���� �� ������
	//

	void PlaceUpperCorners();	// ������ 3 ���� �������
	void RotateUntilRightCorner(color up_col);	// ���� ������ ���
	
	void SetUpperCorners();		// ����������� ���� �� ������

	//
	// ������� ����
	//

	void nRURU();	// "��������" ���-���
	
	void RotateUpperCorners();

};

//class CFOP : public tripleSolver
//{
//public:
//	CFOP(Triple& _cb) {};
//
//	virtual bool solve() {}
//	virtual const stepList& getsolution() const {}
//
//private:
// 
//
//};

#endif // !__SOLVER_H__