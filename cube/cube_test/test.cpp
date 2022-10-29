#include "gtest/gtest.h"

#include "../cube/reader.hpp"
#include "../cube/printer.hpp"
#include "../cube/solver.hpp"
#include "../cube/interface.hpp"


using namespace std;

string goodInp = "yyyyyyyyy\n"
				 "bbbbbbbbb\n"
				 "rrrrrrrrr\n"
				 "ggggggggg\n"
				 "ooooooooo\n"
				 "wwwwwwwww\n";

string anotherGoodInp = "yyyyyyyyy_andSomethingelse\n"
						"bbbbbbbbb\n"
						"rrrrrrrrr these cols will be ignored\n"
						"ggggggggg\n"
						"ooooooooo\n"
						"wwwwwwwww\n"
						"this row will be ignored";

string solvableInp = "bggyyyoyb\n"
					  "ogwrbrybb\n"
					  "bbwgryyrr\n"
					  "rrwogoygw\n"
					  "ooybobrog\n"
					  "rwgwwwowg\n";

string unsolvableInp =  "bggyyyoyb\n"
						"ogwrbrybb\n"
						"bbwgryyrr\n"
						"rrwogoygw\n"
						"ooybobrog\n"
						"rwgwwwogw\n";	// В конце поменяны w и g


string badInp = "y yyyy\n"
				"bbbbbbbbb\n"
				"rrrrrrrrr\n";


string anotherBadInp = "asdasdasd\n"
					   "bbbbbbbbb\n"
					   "rrrrrrrrr\n"
					   "ggggggggg\n"
					   "ooooooooo\n"
					   "wwwwwwwww\n";

string nRubiksInp = "yyyoooyyy\n"
					"bbbbbbbbb\n"
					"rrrrrrrrr\n"
					"ggggggggg\n"
					"ooooooooo\n"
					"wwwwwwwww\n";


TEST(Cube, EmptyCtr) {
	tripleCube cb;
	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());
	ASSERT_EQ(cb.dimension(), 3);
}

TEST(Cube, BadPutPlate) {
	tripleCube cb;
	
	cb.putPlate(0, 1, red);

	ASSERT_FALSE(cb.isRubiks());
	ASSERT_FALSE(cb.isSolved());
}

TEST(Cube, NotrubiksAfterPut) {
	tripleCube cb;

	cb.putPlate(0, 1, red);
	cb.putPlate(2, 1, yellow);


	ASSERT_TRUE(cb.isRubiks());
	ASSERT_FALSE(cb.isSolved());
}

TEST(Cube, PutGet) {
	tripleCube cb;

	cb.putPlate(0, 1, red);
	cb.putPlate(2, 1, yellow);

	ASSERT_EQ(cb.getPlate(0, 1), red);
	ASSERT_EQ(cb.getPlate(2, 1), yellow);
}

TEST(Cube, CopyCtr) {
	tripleCube cb;

	cb.putPlate(0, 1, red);
	cb.putPlate(2, 1, yellow);

	tripleCube sCb(cb);

	for (size_t i = 0; i < 6; i++)
		for (size_t j = 0; j < 9; j++)
			ASSERT_EQ(cb.getPlate(i, j), sCb.getPlate(i, j));
}

TEST(Cube, AllAreChanging) {
	tripleCube cb;
	istringstream iss(goodInp);
	rowReader rd(iss);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	ASSERT_NO_THROW(rd.read(cb));



	for (size_t i = 0; i < 12; i++) {
		cb.doMovement((movement)i, Rand);
		cb.doMovement((movement)i, Rand);
		cb.doMovement((movement)i, Rand);

		ASSERT_TRUE(cb.isRubiks());
		ASSERT_FALSE(cb.isSolved());

		cb.doMovement((movement)i, Rand);

		ASSERT_TRUE(cb.isRubiks());
		ASSERT_TRUE(cb.isSolved());
	}
}

TEST(Cube, BadPutArgs) {
	tripleCube cb;
	ASSERT_THROW(cb.putPlate(0, 9, yellow), invalid_argument);
	ASSERT_THROW(cb.putPlate(6, 0, yellow), invalid_argument);
}

TEST(Cube, BadGetArgs) {
	tripleCube cb;
	ASSERT_THROW(cb.getPlate(0, 9), invalid_argument);
	ASSERT_THROW(cb.getPlate(6, 0), invalid_argument);
}

TEST(Cube, RIsChanging) {	// по аналогии работает со всеми
	tripleCube cb;

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	cb.R(fwd, Rand);
	cb.R(fwd, Rand);
	cb.R(fwd, Rand);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_FALSE(cb.isSolved());

	cb.R(fwd, Rand);
	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());
}

TEST(Cube, RURU) {	// 6 пиф-пафов не меняют куб
	tripleCube cb;

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	for (size_t i = 0; i < 6; i++) {
		cb.R(fwd, Rand);
		cb.U(fwd, Rand);
		cb.R(bcwd, Rand);
		cb.U(bcwd, Rand);
	}

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());
}

TEST(Cube, StepList) {
	tripleCube cb;

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	cb.R(fwd, Rand);
	cb.U(fwd, Rand);
	cb.R(bcwd, Rand);
	cb.U(bcwd, Rand);

	stepList expected = { {r, Rand}, {u, Rand}, {br, Rand}, {bu, Rand} };

	ASSERT_EQ(cb.getSteps(), expected);
}

TEST(Reader, GoodInp) {
	tripleCube cb;
	istringstream iss(goodInp);
	rowReader rd(iss);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	ASSERT_NO_THROW(rd.read(cb));
}

TEST(Reader, AnotherGoodInp) {
	tripleCube cb;
	istringstream iss(anotherGoodInp);
	rowReader rd(iss);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	ASSERT_NO_THROW(rd.read(cb));
}

TEST(Reader, BadInp) {
	tripleCube cb;
	istringstream iss(badInp);
	rowReader rd(iss);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	ASSERT_THROW(rd.read(cb), std::invalid_argument);
}

TEST(Reader, AnotherBadInp) {
	tripleCube cb;
	istringstream iss(anotherBadInp);
	rowReader rd(iss);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	ASSERT_THROW(rd.read(cb), std::invalid_argument);
}

TEST(Reader, NRubiksInp) {
	tripleCube cb;
	std::istringstream iss(nRubiksInp);
	rowReader rd(iss);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	ASSERT_NO_THROW(rd.read(cb));
	
	ASSERT_FALSE(cb.isRubiks());
	ASSERT_FALSE(cb.isSolved());
}

TEST(Reader, DestroyedInp) {
	tripleCube cb;
	std::istringstream iss(solvableInp);
	rowReader rd(iss);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_TRUE(cb.isSolved());

	ASSERT_NO_THROW(rd.read(cb));

	ASSERT_EQ(cb.getPlate(1, 1), green);	// Выбрал рандомный блок
}

// TEST(Printer, InvalidArgs) {
//	simpleConsolePrinter rdr;
//	tripleCube cb;
//	
//	int maxX, maxY;
//	con_getMaxXY(&maxX, &maxY);
//
//
//	ASSERT_THROW(rdr.charAt(' ', 0, maxX+2, maxY), invalid_argument);
//	ASSERT_THROW(rdr.PrintFrame(cb, maxX + 1, maxY, ' ', 0, 0), invalid_argument);
//	ASSERT_THROW(rdr.PrintSide(cb, 6, 0, 0, ' '), invalid_argument);
//	ASSERT_THROW(rdr.PrintSide(cb, 0, maxX + 1, maxY, ' '), invalid_argument);
//	ASSERT_THROW(rdr.printCube(cb, maxX + 1, maxY, ' '), invalid_argument);
//}

TEST(Solver, SolvableInp) {
	tripleCube cb;
	istringstream iss(solvableInp);
	rowReader rdr(iss);
	rdr.read(cb);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_FALSE(cb.isSolved());

	beginnerSolver slvr(cb);

	ASSERT_TRUE(slvr.solve());
	ASSERT_TRUE(cb.isSolved());
}

TEST(Solver, UnolvableInp) {
	tripleCube cb;
	istringstream iss(unsolvableInp);
	rowReader rdr(iss);
	rdr.read(cb);

	ASSERT_TRUE(cb.isRubiks());
	ASSERT_FALSE(cb.isSolved());

	beginnerSolver slvr(cb);

	ASSERT_FALSE(slvr.solve());
}

TEST(InterfacedReader, Ctr) {
	tripleCube cb;
	interfacedReader irdr(cb);
}

TEST(SolutionPrinter, Ctr) {
	tripleCube cb;
	stepList sl;
	solutionPrinter prtr(sl, cb);
}

TEST(Randomizer, CtrAndUse) {
	tripleCube cb;
	randomizer rander(cb);
	ASSERT_NO_THROW(rander.randomize());
}

TEST(Interfacer, Ctr) {
	interfacer itfsr;
}

TEST(Interfacer, InterruptEsc) {
	interfacer itfsr;
	ASSERT_TRUE(itfsr.interrupt(CON_KEY_ESCAPE));
}

TEST(Interfacer, InterruptWrongKey) {
	interfacer itfsr;
	ASSERT_FALSE(itfsr.interrupt(CON_KEY_BACKSPACE));
}