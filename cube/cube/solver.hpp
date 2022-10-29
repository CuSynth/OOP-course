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

	// Обращаем движение
	movement GetReversed(movement mov);

	//
	// Общие вспомогательные функции
	//

	void RURU(step stp);	// Пиф-паф

	bool CheckColor(size_t sd, size_t pos, color col);	// Стоит ли col на месте pos, на стороне sd
	bool CheckTwoOnSide(size_t side, size_t f_pos, size_t s_pos);	// Совпадают ли цвета на стороне sd в местах f_pos, s_pos

	// Вращаем пока на на данном месте стоит данный цвет
	void UUntilPlateHaveColor(direction dir, step stp,
		size_t side, size_t pos, color col, bool cond);

	// Вращаем, пока не найдем данное ребро
	void UUntilNotEdge(direction dir, step stp,
		size_t num, color f_col, color s_col);

	//
	// Крест:
	//

	bool EdgeHaveColor(size_t num, color col);	// Есть ли col на ребое num
	bool CheckEdge(size_t num, color f_col, color s_col);	// Есть ли f_col, s_col на num

	void SolveCross();

	//
	// Нижний слой:
	//

	bool CornerHaveColor(size_t num, color col);	// Есть ли col на угле num
	bool CheckCorner(size_t num, color f_col, color s_col, color t_col);	//	Есть ли f_col, s_col, t_col на угле num
	
	// Вращаем, пока есть/нет цвета на данном ребре
	void UUntilCornerHaveColor(direction dir, step stp,
		size_t num, color col, bool cond);


	void SolveFirstLayer();

	//
	// Второй слой
	//

	void UtoR(step stp);	// Опускаем ребро направо
	void UtoRr(step stp);	// Опускаем с разворотом
	
	// Вращаем, пока есть/нет цвета на данном уголке
	void UUntilEdgeHaveColor(direction dir, step stp,
		size_t num, color col, bool cond);

	void SolveSecondLayer();

	//
	// Верхний крест
	//

	void AngleLineCross(step stp);	// Угол->линия->крест->угол.. на верхней стороне
	void SolveUpperCross();		// Собираем крест
	bool CheckUpperColors();	// На местах ли его боковые цвета
	void PlaceUpperColors();	// Меняем местами фронтальное и левое ребра
	
	void SetUpperColors();		// Ставим по местам боковые цвета креста

	//
	// Ставим углы по местам
	//

	void PlaceUpperCorners();	// Меняем 3 угла местами
	void RotateUntilRightCorner(color up_col);	// Ищем нужный куб
	
	void SetUpperCorners();		// Расставляем углы по местам

	//
	// Вращаем углы
	//

	void nRURU();	// "Обратный" пиф-паф
	
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