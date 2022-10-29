#include "gtest/gtest.h"

#include "..\Total control\calculator.hpp"
#include "..\Total control\printer.hpp"

using namespace std;


string GoodInp = "0 - 4 1 Place0\n"
				"0 - 4 1 P l a c e 0";

string BadInp = "0 + 4 1 Place0\n";
string AnotherBadInp = "0 - 4 Place0\n";


string simpleInp = "2021-05-27T23:00:00 - 2021-05-27T23:00:08 0 Place 0\n"
					"2021-05-27T23:00:05 - 2021-05-27T23:00:10 1 Place 0\n";

string spacedInp = "2021-05-27T23:00:00 - 2021-05-27T23:00:08 0   Place 0\n"
					"2021-05-27T23:00:05 - 2021-05-27T23:00:10 1       Place     0\n";

string simpleOut = "Place: Place 0 \n"
					"  Begining: 2021-05-27T23:00:05; Ending: 2021-05-27T23:00:08\n"
					"  Max count: 2; AVG count: 2\n"
					"  People:\n"
					"    0\n"
					"    1\n";


string notSwappedInp = "0 - 4 1 Place0\n"
						"2 - 6 2 Place0\n"
						"6 - 9 19 Place0\n"
						"1 - 7 3 Place0\n"
						"3 - 8 5 Place0\n"
						"5 - 9 6 Place0\n";

string SwappedInp = "0 - 4 1 Place0\n"
					"6 - 9 19 Place0\n"
					"2 - 6 2 Place0\n"
					"1 - 7 3 Place0\n"
					"3 - 8 5 Place0\n"
					"5 - 9 6 Place0\n";

string notSwappedOut = "Place: Place0 \n"
						"  Begining: 2000-12-31T00:00:00; Ending: 2008-12-31T00:00:00\n"
						"  Max count: 5; AVG count: 2\n"
						"  People:\n"
						"    1\n"
						"    2\n"
						"    3\n"
						"    5\n"
						"    6\n"
						"    19\n";

bool operator==(const TimedMan& lhs, const TimedMan& rhs) {
	return lhs.tBegin == rhs.tBegin
		&& lhs.tEnd == rhs.tEnd
		&& lhs.UID == rhs.UID;
}


TEST(Ctrs, EmptyCtr) {
	PlacesDB pDB;
}

TEST(Ctrs, OtherCtr) {
	Parser prs(cin);
	Printer prt(cout);

	PlacesDB pDB;
	Calculator calc(pDB, 0);

}

TEST(DB, DBAdd) {
	PlacesDB pDB;

	std::chrono::system_clock::time_point tp1 = TimeIO::parse_system_clock("2021-05-27T23:00:01");
	std::chrono::system_clock::time_point tp2 = TimeIO::parse_system_clock("2021-05-27T23:00:02");
	std::chrono::system_clock::time_point tp3 = TimeIO::parse_system_clock("2021-05-27T23:00:03");
	std::chrono::system_clock::time_point tp4 = TimeIO::parse_system_clock("2021-05-27T23:00:04");


	TimedMan man0 = { 01, tp1, tp2};
	TimedMan man1 = { 11, tp4, tp3};

	pDB.putMan("place 0", man0);
	pDB.putMan("place 1", man1);

	auto tmp = pDB.getPlaces();
	ASSERT_EQ(man0, tmp["place 0"][0]);
	ASSERT_EQ(man1, tmp["place 1"][0]);

}

TEST(_Parser, _GoodInp) {
	istringstream iss(GoodInp);
	
	PlacesDB pDB;
	Parser prs(iss);

	ASSERT_NO_THROW(prs.parse(pDB));
}

TEST(_Parser, _BadInp) {
	PlacesDB pDB;

	istringstream iss(BadInp);
	Parser prs(iss);
	ASSERT_THROW(prs.parse(pDB), invalid_argument);

	
	istringstream AnotherIss(AnotherBadInp);
	Parser AnotherPrs(AnotherIss);
	ASSERT_THROW(AnotherPrs.parse(pDB), invalid_argument);
}


TEST(Full, SimpleDat) {
	istringstream iss(simpleInp);
	ostringstream oss;

	PlacesDB pDB;
	Parser prs(iss);
	prs.parse(pDB);

	Calculator calc(pDB, 2);
	calc.Calculate();
	
	Printer prt(oss);
	prt.print(calc.GetCrowds());
	ASSERT_EQ(oss.str(), simpleOut);	
}


TEST(Full, SpacedPlace) {
	istringstream iss(spacedInp);
	ostringstream oss;

	PlacesDB pDB;
	Parser prs(iss);
	prs.parse(pDB);

	Calculator calc(pDB, 2);
	calc.Calculate();

	Printer prt(oss);
	prt.print(calc.GetCrowds());
	ASSERT_EQ(oss.str(), simpleOut);
}

TEST(Full, SwapPeople) {
	istringstream iss(notSwappedInp);
	ostringstream oss;

	PlacesDB pDB;
	Parser prs(iss);
	prs.parse(pDB);

	Calculator calc(pDB, 2);
	calc.Calculate();

	Printer prt(oss);
	prt.print(calc.GetCrowds());

	
	istringstream anotherIss(SwappedInp);
	ostringstream anotherOss;

	PlacesDB anotherPDB;
	Parser anotherPrs(anotherIss);
	anotherPrs.parse(anotherPDB);

	Calculator anotherCalc(anotherPDB, 2);
	anotherCalc.Calculate();

	Printer anotherPrt(anotherOss);
	anotherPrt.print(anotherCalc.GetCrowds());

	ASSERT_EQ(oss.str(), notSwappedOut);
	ASSERT_EQ(anotherOss.str(), notSwappedOut);

}