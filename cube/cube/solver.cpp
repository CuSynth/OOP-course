#include "solver.hpp"

const size_t up = 0;
const size_t left = 1;
const size_t front = 2;
const size_t right = 3;
const size_t back = 4;
const size_t down = 5;

bool beginnerSolver::solve() {
	toSolve.resetSteps();
	SolveCross();
	SolveFirstLayer();
	SolveSecondLayer();
	SolveUpperCross();
	SetUpperColors();
	SetUpperCorners();
	RotateUpperCorners();
	optimize();	//	До 15% уменьшение длины решения :)

	return toSolve.isSolved();
};

void beginnerSolver::optimize() {
	stepList& solution = toSolve.getSteps();

	auto preEnd = --solution.end();
	auto currentIter = solution.begin();
	auto previousIter = currentIter;
	movement previousMov = previousIter->first;

	size_t counter = 0;

	for (; currentIter != preEnd; currentIter++) {
	
		if (counter == 3 && currentIter->first == previousMov) {
			solution.erase(previousIter, (++currentIter));
			previousIter = currentIter;
			previousMov = previousIter->first;
			counter = 0;

		}
		else if(counter == 3) {
			stepList::iterator cur = currentIter;
			solution.erase(previousIter, --cur);
			cur->first = GetReversed(cur->first);
			currentIter = cur;
			previousIter = cur;
			previousMov = previousIter->first;
			counter = 0;
		}

		if (currentIter->first != previousMov) {
			counter = 1;
			previousIter = currentIter;
			previousMov = previousIter->first;
		}
		else {
			counter++;
		}
	}
}

const stepList& beginnerSolver::getsolution() const { 
	return toSolve.getSteps(); 
}

movement beginnerSolver::GetReversed(movement mov) {
	if (mov % 2 == 0)
		return (movement)(mov + 1);
	return (movement)(mov - 1);
}
// Далее много логики, самый простой метод сборки

//
// Общие вспомогательные
//

void beginnerSolver::RURU(step sol_step) {
	toSolve.R(fwd, sol_step);
	toSolve.U(fwd, sol_step);
	toSolve.R(bcwd, sol_step);
	toSolve.U(bcwd, sol_step);
}

bool beginnerSolver::CheckColor(size_t side, size_t position, color color) {
	return toSolve.getPlate(side, position) == color;
}

bool beginnerSolver::CheckTwoOnSide(size_t side, size_t f_position, size_t s_position) {
	return toSolve.getPlate(side, f_position) == toSolve.getPlate(side, s_position);
}

void beginnerSolver::UUntilPlateHaveColor( direction _direction, step _step,
	size_t side, size_t position, color color, bool condition) {
	for (size_t i = 0; i < 4 && (CheckColor(side, position, color) == condition); i++) {
		toSolve.U(_direction, _step);
	}
}

void beginnerSolver::UUntilNotEdge(direction _direction, step _step,
	size_t num, color firstColor, color secondColor) {

	for (size_t i = 0; i < 4 && !CheckEdge(num, firstColor, secondColor); i++) {
		toSolve.U(_direction, _step);
	}
}

//
// Крест
//

bool beginnerSolver::EdgeHaveColor(size_t num, color _color) {
	if (num > 11)
		return false;

	size_t sides[][2] = { {0, 4}, {0, 1}, {0, 3}, {0, 2}, {1, 4}, {3, 4}, {1, 2}, {2, 3}, {4, 5}, {1, 5}, {3, 5}, {1, 5} };
	size_t pos_s[][2] = { {1, 1}, {3, 1}, {5, 1}, {7, 1}, {3, 5}, {5, 3}, {5, 3}, {5, 3}, {7, 7}, {7, 3}, {7, 5}, {7, 1} };

	return CheckColor(sides[num][0], pos_s[num][0], _color)
		|| CheckColor(sides[num][1], pos_s[num][1], _color);
}

bool beginnerSolver::CheckEdge(size_t num, color firstColor, color secondColor) {
	return EdgeHaveColor(num, firstColor)
		&& EdgeHaveColor(num, secondColor);
}

void beginnerSolver::SolveCross() {
	color downColor = toSolve.getPlate(down, 4);

	for (size_t i = 0; i < 4; i++, toSolve.U(fwd, Cross)) {
		if (CheckTwoOnSide(up, 4, 7))
			continue;
		if (toSolve.getPlate(front, 1) == downColor) {
			toSolve.F(fwd, Cross);
			toSolve.U(bcwd, Cross);
			toSolve.R(fwd, Cross);
			toSolve.U(fwd, Cross);
		}
	}
	UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, false);
	UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, true);

	for (size_t i = 0; i < 4; i++, toSolve.hM(fwd, Cross)) {
		if (toSolve.getPlate(2, 5) == downColor) {
			toSolve.U(bcwd, Cross);
			toSolve.R(fwd, Cross);
			toSolve.U(fwd, Cross);
			toSolve.U(fwd, Cross);
			i--;
			toSolve.hM(bcwd, Cross);
			UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, true);
			UUntilPlateHaveColor(fwd, Cross, front, 1, downColor, true);

		}
		else if (toSolve.getPlate(3, 3) == downColor) {
			toSolve.F(bcwd, Cross);
			toSolve.U(fwd, Cross);
			i--;
			toSolve.hM(bcwd, Cross);
			UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, true);
			UUntilPlateHaveColor(fwd, Cross, front, 1, downColor, true);
		}

	}

	UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, false);
	UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, true);

	for (size_t i = 0; i < 4; i++, toSolve.D(bcwd, Cross)) {
		if (toSolve.getPlate(5, 1) == downColor) {
			toSolve.F(fwd, Cross);
			toSolve.F(fwd, Cross);
			toSolve.U(fwd, Cross);
			UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, true);
			UUntilPlateHaveColor(fwd, Cross, front, 1, downColor, true);
		}
		else if (toSolve.getPlate(front, 7) == downColor) {
			toSolve.F(bcwd, Cross);
			toSolve.U(bcwd, Cross);
			toSolve.D(fwd, Cross);
			toSolve.R(fwd, Cross);
			toSolve.D(bcwd, Cross);
			UUntilPlateHaveColor(fwd, Cross, up, 7, downColor, true);
			UUntilPlateHaveColor(fwd, Cross, front, 1, downColor, true);
		}

	}

	for (size_t i = 0; i < 4; i++) {
		color col = toSolve.getPlate(front, 4);
		UUntilNotEdge(fwd, Cross, 3, downColor, col);
		toSolve.F(fwd, Cross);
		toSolve.F(fwd, Cross);
		toSolve.hM(fwd, Cross);
		toSolve.D(bcwd, Cross);
	}
}

//
// Нижний слой:
//

bool beginnerSolver::CornerHaveColor(size_t num, color _color) {

	// Стороны и положения цветов относительно номера угла
	size_t sides[][3] = { {0, 1, 4}, {0, 3, 4}, {0, 1, 2}, {0, 2, 3}, {1, 4, 5}, {3, 4, 5}, {1, 2, 5}, {2, 3, 5} };
	size_t pos_s[][3] = { {0, 0, 2}, {2, 2, 0}, {6, 2, 0}, {8, 2, 0}, {6, 8, 6}, {8, 6, 8}, {8, 6, 0}, {8, 6, 2} };

	return CheckColor(sides[num][0], pos_s[num][0], _color)
		|| CheckColor(sides[num][1], pos_s[num][1], _color)
		|| CheckColor(sides[num][2], pos_s[num][2], _color);
}

bool beginnerSolver::CheckCorner(size_t num, color firstColor, color secondColor, color thirdColor) {
	return CornerHaveColor(num, firstColor)
		&& CornerHaveColor(num, secondColor)
		&& CornerHaveColor(num, thirdColor);
}

void beginnerSolver::UUntilCornerHaveColor(direction _direction, step _step,
	size_t num, color _color, bool cond) {

	for (size_t i = 0; i < 4 && (CornerHaveColor(num, _color)) == cond; i++)
		toSolve.U(_direction, _step);
}

void beginnerSolver::SolveFirstLayer() {
	color downColor = toSolve.getPlate(down, 4);
	for (size_t i = 0; i < 4; i++) {
		if (CornerHaveColor(7, downColor)) {
			UUntilCornerHaveColor(fwd, First_Layer, 3, downColor, true);
			RURU(First_Layer);
		}
		toSolve.D(bcwd, First_Layer);
	}

	for (size_t i = 0; i < 4; i++) {
		color f_col = toSolve.getPlate(front, 7);
		color s_col = toSolve.getPlate(right, 7);

		for (size_t i = 0; i < 4 && !CheckCorner(3, f_col, s_col, downColor); i++)
			toSolve.U(fwd, First_Layer);

		for (size_t i = 0; i < 6 && (toSolve.getPlate(down, 2) != downColor); i++)
			RURU(First_Layer);

		toSolve.D(bcwd, First_Layer);
	}
}

//
// Второй слой
//

void beginnerSolver::UtoR(step _step) {
	toSolve.U(fwd, _step);
	toSolve.R(fwd, _step);
	toSolve.U(bcwd, _step);
	toSolve.R(bcwd, _step);
	toSolve.U(bcwd, _step);
	toSolve.F(bcwd, _step);
	toSolve.U(fwd, _step);
	toSolve.F(fwd, _step);
}

void beginnerSolver::UtoRr(step _step) {
	toSolve.U(bcwd, _step);
	toSolve.U(bcwd, _step);
	toSolve.F(bcwd, _step);
	toSolve.U(bcwd, _step);
	toSolve.F(fwd, _step);
	toSolve.U(fwd, _step);
	toSolve.R(fwd, _step);
	toSolve.U(fwd, _step);
	toSolve.R(bcwd, _step);
}

void beginnerSolver::UUntilEdgeHaveColor(direction _direction, step _step,
	size_t num, color _color, bool cond) {

	for (size_t i = 0; i < 4 && (EdgeHaveColor(num, _color)) == cond; i++)
		toSolve.U(_direction, _step);
}

void beginnerSolver::SolveSecondLayer() {
	color upperColor = toSolve.getPlate(up, 4);
	for (size_t i = 0; i < 4; i++) {
		if (!EdgeHaveColor(7, upperColor)) {
			UUntilEdgeHaveColor(fwd, Second_Layer, 3, upperColor, false);
			UtoR(Second_Layer);
		}
		toSolve.hM(fwd, Second_Layer);
	}

	for (size_t i = 0; i < 4; i++) {
		color f_col = toSolve.getPlate(front, 4);
		color s_col = toSolve.getPlate(right, 4);

		UUntilNotEdge(fwd, Second_Layer, 3, f_col, s_col);

		if (toSolve.getPlate(2, 1) == f_col) {
			UtoR(Second_Layer);
		}
		else {
			UtoRr(Second_Layer);
		}
		toSolve.hM(fwd,Second_Layer);
	}
}

//
// Верхний крест
//

void beginnerSolver::AngleLineCross(step _step) {
	toSolve.F(fwd, _step);
	RURU(_step);
	toSolve.F(bcwd, _step);
}

void beginnerSolver::SolveUpperCross() {
	color upperColor = toSolve.getPlate(up, 4);
	UUntilPlateHaveColor(fwd, Second_Cross, up, 7, upperColor, false);
	if (toSolve.getPlate(0, 7) != upperColor) {
		AngleLineCross(Second_Cross);
		UUntilPlateHaveColor(fwd, Second_Cross,up, 7, upperColor, false);
	}
	UUntilPlateHaveColor(fwd,Second_Cross, up, 7, upperColor, true);

	if (toSolve.getPlate(up, 7) == upperColor) {
		return;
	}
	if (toSolve.getPlate(up, 1) == upperColor) {
		AngleLineCross(Second_Cross);
		AngleLineCross(Second_Cross);
	}
	else {
		AngleLineCross(Second_Cross);
	}
}

bool beginnerSolver::CheckUpperColors() {
	for (size_t i = 1; i < 5; i++)
		if (toSolve.getPlate(i, 1) != toSolve.getPlate(i, 4))
			return false;
	return true;
}

void beginnerSolver::PlaceUpperColors() {
	toSolve.R(fwd, Second_Cross);
	toSolve.U(fwd, Second_Cross);
	toSolve.R(bcwd, Second_Cross);
	toSolve.U(fwd, Second_Cross);
	toSolve.R(fwd, Second_Cross);
	toSolve.U(fwd, Second_Cross);
	toSolve.U(fwd, Second_Cross);
	toSolve.R(bcwd, Second_Cross);
}

void beginnerSolver::SetUpperColors() {
	color checkColor = toSolve.getPlate(front, 4);
	UUntilPlateHaveColor(fwd, Upper_Colors, front, 1, checkColor, false);

	if (CheckUpperColors())
		return;

	if (CheckTwoOnSide(left, 1, 4)) {
		toSolve.U(fwd, Second_Cross);
		toSolve.U(fwd, Second_Cross);
		PlaceUpperColors();
		toSolve.U(bcwd, Second_Cross);
	}
	else if (CheckTwoOnSide(back, 1, 4)) {
		PlaceUpperColors();
		toSolve.U(bcwd, Second_Cross);
		PlaceUpperColors();
		toSolve.U(fwd, Second_Cross);
		toSolve.U(fwd, Second_Cross);

	}
	else if (CheckTwoOnSide(right, 1, 4)) {
		toSolve.U(bcwd, Second_Cross);
		PlaceUpperColors();
		toSolve.U(fwd, Second_Cross);
		toSolve.U(fwd, Second_Cross);
	}
	else {
		checkColor = toSolve.getPlate(back, 4);
		UUntilPlateHaveColor(fwd, Upper_Colors, back, 1, checkColor, false);
		if (CheckTwoOnSide(left, 1, 4)) {
			toSolve.U(fwd, Second_Cross);
			PlaceUpperColors();
		}
		else if (CheckTwoOnSide(right, 1, 4)) {
			PlaceUpperColors();
			toSolve.U(fwd, Second_Cross);
		}
		else {
			toSolve.U(fwd, Second_Cross);
			PlaceUpperColors();
			toSolve.U(bcwd, Second_Cross);
			PlaceUpperColors();
			toSolve.U(bcwd, Second_Cross);
		}
	}
}

//
// Ставим углы по местам
//

void beginnerSolver::PlaceUpperCorners() {
	toSolve.R(fwd, Set_Upper_Corners);
	toSolve.U(bcwd, Set_Upper_Corners);
	toSolve.L(bcwd, Set_Upper_Corners);
	toSolve.U(fwd, Set_Upper_Corners);
	toSolve.R(bcwd, Set_Upper_Corners);
	toSolve.U(bcwd, Set_Upper_Corners);
	toSolve.L(fwd, Set_Upper_Corners);
	toSolve.U(fwd, Set_Upper_Corners);
}

void beginnerSolver::RotateUntilRightCorner(color up_col) {
	for (size_t i = 0; i < 4; i++) {
		color f_col = toSolve.getPlate(front, 1);
		color s_col = toSolve.getPlate(right, 1);
		if (CheckCorner(3, f_col, s_col, up_col))
			break;
		toSolve.U(fwd, Set_Upper_Corners);
	}
}

void beginnerSolver::SetUpperCorners() {
	color upperColor = toSolve.getPlate(up, 4);

	RotateUntilRightCorner( upperColor);

	color f_col = toSolve.getPlate(left, 1);
	color s_col = toSolve.getPlate(front, 1);

	if (CheckCorner(2, f_col, s_col, upperColor))
		return;

	f_col = toSolve.getPlate(front, 1);
	s_col = toSolve.getPlate(right, 1);

	if (!CheckCorner(3, f_col, s_col, upperColor)) {
		PlaceUpperCorners();
		RotateUntilRightCorner(upperColor);
	}

	toSolve.U(fwd, Set_Upper_Corners);
	PlaceUpperCorners();

	f_col = toSolve.getPlate(front, 1);
	s_col = toSolve.getPlate(right, 1);

	if (!CheckCorner(3, f_col, s_col, upperColor)) {
		PlaceUpperCorners();
	}
	f_col = toSolve.getPlate(front, 4);

	UUntilPlateHaveColor(fwd,Set_Upper_Corners,front, 1, f_col, false);
}

//
// Вращаем углы
//

void beginnerSolver::nRURU() {
	toSolve.L(fwd, Rotate_Upper_Corners);
	toSolve.D(fwd, Rotate_Upper_Corners);
	toSolve.L(bcwd, Rotate_Upper_Corners);
	toSolve.D(bcwd, Rotate_Upper_Corners);
}

void beginnerSolver::RotateUpperCorners() {
	color upperColor = toSolve.getPlate(up, 4);
	for (size_t i = 0; i < 4; i++) {
		for (size_t i = 0; i < 6 && (toSolve.getPlate(up, 6) != upperColor); i++)
			nRURU();
		toSolve.U(fwd, Rotate_Upper_Corners);
	}
}