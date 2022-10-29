#include "cube.hpp"

tripleCube::tripleCube() {
	for (size_t i = 0; i < 6; ++i) {
		for (size_t j = 0; j < 9; j++) {
			body[i][j] = (color)i;
		}
	}
}

bool tripleCube::isSolved() const {
	for (size_t i = 0; i < 6; i++) {
		color pivot = body[i][4];

		for (size_t j = 0; j < 9; j++)
			if (body[i][j] != pivot)
				return false;
	}

	return true;
}

bool tripleCube::isRubiks() const {
	size_t counters[] = { 0, 0, 0, 0, 0, 0 };

	for (size_t i = 0; i < 6; i++)
		for (size_t j = 0; j < 9; j++) {
			++counters[(size_t)body[i][j]];
		}

	for (auto it : counters)
		if (it != 9)
			return false;					// Несоответствие количества плиток с цветом
	
	return true;
}

size_t tripleCube::dimension()  const {
	return 3;
}

void tripleCube::putPlate(size_t side, size_t position, color _color) {
	if (side > 5 || position > 8)
		throw std::invalid_argument( "Wrong param while putting plate to cube!\n" );

	body[side][position] = _color;
}

color tripleCube::getPlate(size_t side, size_t position) const {
	if (side > 5 || position > 8)
		throw std::invalid_argument( "Wrong param while getting plate from cube!\n" );
	return body[side][position];
}

void tripleCube::doMovement(movement _movement, step _step) {
	switch (_movement)
	{
	case r:
		R(fwd, _step);
		break;
	case br:
		R(bcwd, _step);
		break;
	case l:
		L(fwd, _step);
		break;
	case bl:
		L(bcwd, _step);
		break;
	case f:
		F(fwd, _step);
		break;
	case bf:
		F(bcwd, _step);
		break;
	case b:
		B(fwd, _step);
		break;
	case bb:
		B(bcwd, _step);
		break;
	case u:
		U(fwd, _step);
		break;
	case bu:
		U(bcwd, _step);
		break;
	case d:
		D(fwd, _step);
		break;
	case bd:
		D(bcwd, _step);
		break;
	case hm:
		hM(fwd, _step);
		break;
	case bhm:
		hM(bcwd, _step);
		break;
	default:
		break;
	}
}



void tripleCube::F(direction _direction, step _step) {
	if (_direction == fwd) {
		RowToCol(0, 2, 1, 2);
		RowToCol(5, 0, 1, 2);
		RowToCol(5, 0, 3, 0);
	}
	else {
		RowToCol(0, 2, 3, 0);
		RowToCol(5, 0, 3, 0);
		RowToCol(5, 0, 1, 2);
	}

	SideRotation(2, _direction);
	steps.push_back({ _direction == fwd ? f : bf, _step });
}

void tripleCube::R(direction _direction, step _step) {
	if (_direction == fwd) {
		ColToCol(0, 2, 2, 2);
		ColToCol(5, 2, 2, 2);
		ColToCol(5, 2, 4, 0);
	}
	else {
		ColToCol(0, 2, 4, 0);
		ColToCol(5, 2, 4, 0);
		ColToCol(5, 2, 2, 2);
	}

	SideRotation(3, _direction);
	steps.push_back({ _direction == fwd ? r : br, _step });
}

void tripleCube::L(direction _direction, step _step) {
	if (_direction == fwd) {
		ColToCol(0, 0, 4, 2);
		ColToCol(5, 0, 4, 2);
		ColToCol(5, 0, 2, 0);
	}
	else {
		ColToCol(0, 0, 2, 0);
		ColToCol(5, 0, 2, 0);
		ColToCol(5, 0, 4, 2);
	}

	SideRotation(1, _direction);
	steps.push_back({ _direction == fwd ? l : bl, _step });
}

void tripleCube::B(direction _direction, step _step) {
	if (_direction == fwd) {
		RowToCol(0, 0, 3, 2);
		RowToCol(5, 2, 3, 2);
		RowToCol(5, 2, 1, 0); 

	}
	else {
		RowToCol(0, 0, 1, 0);
		RowToCol(5, 2, 1, 0);
		RowToCol(5, 2, 3, 2);
	}

	SideRotation(4, _direction);
	steps.push_back({ _direction == fwd ? b : bb, _step });
}

void tripleCube::U(direction _direction, step _step) {
	if (_direction == fwd) {
		RowToRow(2, 0, 3, 0);
		RowToRow(4, 0, 3, 0);
		RowToRow(4, 0, 1, 0);
	}
	else {
		RowToRow(2, 0, 1, 0);
		RowToRow(4, 0, 1, 0);
		RowToRow(4, 0, 3, 0);
	}

	SideRotation(0, _direction);
	steps.push_back({ _direction == fwd ? u : bu, _step });
}

void tripleCube::D(direction _direction, step _step) {
	if (_direction == fwd) {
		RowToRow(2, 2, 1, 2);
		RowToRow(4, 2, 1, 2);
		RowToRow(4, 2, 3, 2);
	}
	else {
		RowToRow(2, 2, 3, 2);
		RowToRow(4, 2, 3, 2);
		RowToRow(4, 2, 1, 2);
	}

	SideRotation(5, _direction);
	steps.push_back({ _direction == fwd ? d : bd, _step });
}

void tripleCube::hM(direction _direction, step _step) {
	if (_direction == fwd) {
		RowToRow(2, 1, 3, 1);
		RowToRow(4, 1, 3, 1);
		RowToRow(4, 1, 1, 1);
	}
	else {
		RowToRow(2, 1, 1, 1);
		RowToRow(4, 1, 1, 1);
		RowToRow(4, 1, 3, 1);
	}
	steps.push_back({ _direction == fwd ? hm : bhm, _step });
}

void tripleCube::resetSteps() {
	steps.clear();
}

const stepList& tripleCube::getSteps() const { return steps; }

stepList& tripleCube::getSteps() { return steps; }



void tripleCube::SwapPlates(size_t firstSide, size_t firstPos,
	size_t secondSide, size_t secondPos) {

	if (firstSide > 5 || firstPos > 8 || secondSide > 5 || secondPos > 8)
		throw std::invalid_argument( "Wrong param while cube rotation!\n" );

	color tmp = body[firstSide][firstPos];
	body[firstSide][firstPos] = body[secondSide][secondPos];
	body[secondSide][secondPos] = tmp;
}

void tripleCube::SideRotation(size_t side, direction _direction) {
	if (side > 5)
		throw std::invalid_argument( "Wrong param while cube rotation!\n" );

	if (_direction == fwd) {
		SwapPlates(side, 0, side, 6);
		SwapPlates(side, 6, side, 8);
		SwapPlates(side, 8, side, 2);

		SwapPlates(side, 1, side, 3);
		SwapPlates(side, 3, side, 7);
		SwapPlates(side, 7, side, 5);
	}
	else {
		SwapPlates(side, 0, side, 2);
		SwapPlates(side, 2, side, 8);
		SwapPlates(side, 8, side, 6);

		SwapPlates(side, 1, side, 5);
		SwapPlates(side, 5, side, 7);
		SwapPlates(side, 7, side, 3);
	}
}


void  tripleCube::RowToRow(size_t fSide, size_t fRowNum, size_t sSide, size_t sRowNum) {
	if (fSide > 5 || fRowNum > 2 || sSide > 5 || sRowNum > 2)
		throw std::invalid_argument( "Wrong param while cube rotation!\n" );

	for (size_t i = 0; i < 3; i++) {
		SwapPlates(fSide, 3 * fRowNum + i, sSide, 3 * sRowNum + i);
	}
}

void  tripleCube::ColToCol(size_t fSide, size_t fColNum, size_t sSide, size_t sColNum) {
	if (fSide > 5 || fColNum > 2 || sSide > 5 || sColNum > 2)
		throw std::invalid_argument( "Wrong param while cube rotation!\n" );

	bool reversed = fSide == 4 || sSide == 4;	// Корректно вращаем заднюю сторону
	
	if (reversed) {
		for (size_t i = 0; i < 3; i++) {
			SwapPlates(fSide, 3 * i + fColNum, sSide, 3 * (2-i) + sColNum);
		}
	} else {
		for (size_t i = 0; i < 3; i++) {
			SwapPlates(fSide, 3 * i + fColNum, sSide, 3 * i + sColNum);
		}
	}
}

void  tripleCube::RowToCol(size_t fSide, size_t RowNum, size_t sSide, size_t ColNum) {
	if (fSide > 5 || RowNum > 2 || sSide > 5 || ColNum > 2)
		throw std::invalid_argument( "Wrong param while cube rotation!\n" );

	bool reversed = (fSide == 0 && sSide == 1) || (fSide == 5 && sSide == 3);	// Корректно вращаем левую/правую стороны

	if (reversed) {
		for (size_t i = 0; i < 3; i++) {
			SwapPlates(fSide, 3 * RowNum + i, sSide, 3 * (2-i) + ColNum);
		}
	}
	else {
		for (size_t i = 0; i < 3; i++) {
			SwapPlates(fSide, 3 * RowNum + i, sSide, 3 * i + ColNum);
		}
	}

}
