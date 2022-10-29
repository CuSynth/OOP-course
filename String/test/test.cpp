#include "gtest/gtest.h"
#include "..\String.hpp"

char tst[] = "test";
char nothing[] = "";

const std::string TEST_STRING = "Test string";
const size_t TEST_SIZE = TEST_STRING.size();

TEST(TestCtr, Ctr) {
	{
		String str;
	}

	{
		String str(5, 'a');
		ASSERT_EQ(str.size(), 5);
		ASSERT_EQ(str.capacity(), 6);
	}

	{
		char tst[] = "TestTastTustTost";
		String str(tst);
		String str2(tst, 8);
		String str3(str2, 0);
		String str4(str2, 4, 4);
	}
}

TEST(TestCtr, NullPtr) {
	String str(nullptr);
	String str2(nullptr, 2);
	String str3(0, 'a');
}

TEST(TestCtrDat, DatSizeEQ) {
	{
		char newTst[] = "TestTastTustTost";
		String str2(newTst);
		ASSERT_STREQ(str2.data(), newTst);
		ASSERT_EQ(str2.size(), 16);
		ASSERT_EQ(str2.capacity(), 17);


		char cutted[] = "TestTastTust";
		String str3(newTst, 12);
		ASSERT_STREQ(str3.data(), cutted);
		ASSERT_EQ(str3.size(), 12);
		ASSERT_EQ(str3.capacity(), 13);

		String str4(str3);
		ASSERT_STREQ(str4.data(), cutted);
		ASSERT_EQ(str4.size(), 12);
		ASSERT_EQ(str4.capacity(), 13);
		ASSERT_EQ(str3.countRef(), 2);
		ASSERT_EQ(str4.countRef(), 2);


		char cutted2[] = "Tast";
		String str5(str4, 4, 4);
		ASSERT_STREQ(str5.data(), cutted2);
		ASSERT_EQ(str5.size(), 4);
		ASSERT_EQ(str5.capacity(), 5);

		char cutted3[] = "TastTust";
		String str6(str4, 4);
		ASSERT_STREQ(str6.data(), cutted3);
		ASSERT_EQ(str6.size(), 8);
		ASSERT_EQ(str6.capacity(), 9);

		ASSERT_STREQ(str4.data(), cutted);
		ASSERT_EQ(str4.size(), 12);
		ASSERT_EQ(str4.capacity(), 13);
	}

	{
		char tst[] = "aaaaa";
		String str3(5, 'a');
		ASSERT_STREQ(str3.data(), tst);
	}
}

TEST(TestFunc, Reserve) {
	{
		String str(tst);
		String str2(str);
		String str3(str2);
		ASSERT_EQ(str.countRef(), 3);

		str2.reserve();
		ASSERT_STREQ(str2.data(), tst);
		ASSERT_EQ(str2.size(), 4);
		ASSERT_EQ(str2.capacity(), 5);
		ASSERT_EQ(str2.countRef(), 3);
		ASSERT_EQ(str.countRef(), 3);

		str3.reserve(10);
		ASSERT_EQ(str3.size(), 4);
		ASSERT_EQ(str3.capacity(), 10);
		ASSERT_EQ(str3.countRef(), 1);
		ASSERT_EQ(str.countRef(), 2);
	}
	

	{
		String nth(nothing);
		nth.reserve(50);
		ASSERT_EQ(nth.size(), 0);
		ASSERT_EQ(nth.capacity(), 50);
		ASSERT_EQ(nth.countRef(), 1);
		ASSERT_STREQ(nth.data(), "");
	}
}

TEST(TestFunc, Clear) {
	char exp[] = "";

	{
		String str(tst);
		str.clear();	
		ASSERT_STREQ(str.data(), exp);
		ASSERT_EQ(str.size(), 0);
		ASSERT_EQ(str.capacity(), 5);
		ASSERT_EQ(str.countRef(), 1);
	}

	{
		String str(tst);
		String str2(str);
		String str3(str);

		str3.clear();

		ASSERT_STREQ(str.data(), tst);
		ASSERT_EQ(str.size(), 4);
		ASSERT_EQ(str.capacity(), 5);
		ASSERT_EQ(str.countRef(), 2);

		ASSERT_STREQ(str3.data(), exp);
		ASSERT_EQ(str3.size(), 0);
		ASSERT_EQ(str3.capacity(), 5);
		ASSERT_EQ(str3.countRef(), 1);
	}


	{
		String nth(nothing);
		nth.clear();
		ASSERT_EQ(nth.size(), 0);
		ASSERT_EQ(nth.capacity(), 1);
		ASSERT_EQ(nth.countRef(), 1);
		ASSERT_STREQ(nth.data(), "");
	}
}

TEST(TestFunc, Empty) {
	{
		String str;
		ASSERT_TRUE(str.empty());
	}

	{
		String str(tst);
		ASSERT_FALSE(str.empty());
	}

	{
		String str(tst);
		str.clear();
		ASSERT_TRUE(str.empty());
	}

	{
		String str(tst);
		String str2(tst);
		str.clear();
		ASSERT_TRUE(str.empty());
		ASSERT_FALSE(str2.empty());
	}


	{
		String nth(nothing);
		nth.clear();
		ASSERT_TRUE(nth.empty());
		ASSERT_EQ(nth.size(), 0);
		ASSERT_EQ(nth.capacity(), 1);
		ASSERT_EQ(nth.countRef(), 1);
		ASSERT_STREQ(nth.data(), "");
	}
}

TEST(TestFunc, at) {
	{
		String str(tst);
		ASSERT_EQ(str.at(0), 't');
		str.at(0) = 'b';
		ASSERT_EQ(str.at(0), 'b');
		ASSERT_STREQ(str.data(), "best");

		ASSERT_THROW(str.at(100), std::out_of_range);
	}

	{
		String str(tst);
		ASSERT_EQ(str[0], 't');
		str[0] = 'b';
		ASSERT_EQ(str[0], 'b');
		ASSERT_STREQ(str.data(), "best");
	
		ASSERT_THROW(str.at(100), std::out_of_range);
	}


	{
		String nth(nothing);
		ASSERT_THROW(nth.at(100), std::out_of_range);

	}
}

TEST(TestFunc, FrontBack) {
	{
		String str(tst);
		ASSERT_EQ(str.front(), 't');
		str.front() = 'b';
		ASSERT_EQ(str.front(), 'b');
		ASSERT_STREQ(str.data(), "best");
	}

	{
		String str(tst);
		ASSERT_EQ(str.back(), 't');
		str.back() = 'o';
		ASSERT_EQ(str.back(), 'o');
		ASSERT_STREQ(str.data(), "teso");
	}


	{
		String nth(nothing);
		ASSERT_THROW(nth.front(), std::out_of_range);
		ASSERT_THROW(nth.back(), std::out_of_range);
	}
}

TEST(TestFunc, Operators) {
	{
		String str(tst);
		String str2(tst);
		str2[0] = 'b';

		str2 += str;

		ASSERT_STREQ(str.data(), tst);
		ASSERT_EQ(str.size(), 4);
		ASSERT_EQ(str.capacity(), 5);
		ASSERT_EQ(str.countRef(), 1);

		ASSERT_STREQ(str2.data(), "besttest");
		ASSERT_EQ(str2.size(), 8);
		ASSERT_EQ(str2.capacity(), 9);
		ASSERT_EQ(str2.countRef(), 1);
	}

	{
		String str(tst);
		str[0] = 'b';

		str += tst;

		ASSERT_STREQ(str.data(), "besttest");
		ASSERT_EQ(str.size(), 8);
		ASSERT_EQ(str.capacity(), 9);
		ASSERT_EQ(str.countRef(), 1);
	}

	{
		String str(tst);
		str += 's';

		ASSERT_STREQ(str.data(), "tests");
		ASSERT_EQ(str.size(), 5);
		ASSERT_EQ(str.capacity(), 6);
		ASSERT_EQ(str.countRef(), 1);
	}


	{
		String str(tst);
		String str2(str);
		String str3(str);

		String nStr(tst);
		nStr += tst;
		String nStr2(nStr);

		ASSERT_EQ(str.countRef(), 3);
		ASSERT_EQ(nStr.countRef(), 2);

		str2 = nStr2;
		ASSERT_EQ(str.countRef(), 2);
		ASSERT_EQ(nStr.countRef(), 3);

		char exp[] = "testtest";

		ASSERT_STREQ(str.data(), tst);
		ASSERT_STREQ(str2.data(), exp);
		ASSERT_STREQ(nStr2.data(), exp);
	}

	{
		String str(tst);
		String str2(str);
		String str3(str);

		str2 = "best";
		ASSERT_EQ(str.countRef(), 2);
		ASSERT_EQ(str2.countRef(), 1);

		ASSERT_STREQ(str.data(), tst);
		ASSERT_STREQ(str2.data(), "best");
	}



	{
		String nth(nothing);
		String str(tst);

		nth += tst;

		ASSERT_EQ(nth.size(), 4);
		ASSERT_EQ(nth.capacity(), 5);
		ASSERT_EQ(nth.countRef(), 1);
		ASSERT_STREQ(nth.data(), tst);

		str += "";
		
		ASSERT_EQ(str.size(), 4);
		ASSERT_EQ(str.capacity(), 5);
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_STREQ(str.data(), tst);

	}

	{
		String nth(nothing);
		nth = tst;
		ASSERT_EQ(nth.size(), 4);
		ASSERT_EQ(nth.capacity(), 5);
		ASSERT_EQ(nth.countRef(), 1);
		ASSERT_STREQ(nth.data(), tst);
	}
}

TEST(TestFunc, Insert) {
	{
		String str(tst);
		String str2(str);
		ASSERT_EQ(str.size(), 4);
		ASSERT_EQ(str.capacity(), 5);

		str2 = "best";


		String newstr = str.insert(2, str2);

		ASSERT_STREQ(newstr.data(), "tebestst");
		ASSERT_STREQ(str2.data(), "best");

		ASSERT_EQ(newstr.countRef(), 2);
		ASSERT_EQ(newstr.size(), 8);
		ASSERT_EQ(newstr.capacity(), 9);

		ASSERT_EQ(str2.countRef(), 1);
		ASSERT_EQ(str2.size(), 4);
		ASSERT_EQ(str2.capacity(), 5);
	}

	{
		String str(tst);
		String newstr = str.insert(4, "er");

		ASSERT_STREQ(newstr.data(), "tester");

		ASSERT_EQ(newstr.countRef(), 2);
		ASSERT_EQ(newstr.size(), 6);
		ASSERT_EQ(newstr.capacity(), 7);
	}

	{
		String str(tst);
		String newstr = str.insert(2, 2, 's');

		ASSERT_STREQ(newstr.data(), "tessst");

		ASSERT_EQ(newstr.countRef(), 2);
		ASSERT_EQ(newstr.size(), 6);
		ASSERT_EQ(newstr.capacity(), 7);
	}


	{
		String nth(nothing);
		String str(tst);

		nth.insert(0, tst);

		ASSERT_EQ(nth.size(), 4);
		ASSERT_EQ(nth.capacity(), 5);
		ASSERT_EQ(nth.countRef(), 1);
		ASSERT_STREQ(nth.data(), tst);

		str.insert(0, "");

		ASSERT_EQ(str.size(), 4);
		ASSERT_EQ(str.capacity(), 5);
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_STREQ(str.data(), tst);
	}
}

TEST(TestFunc, Erase) {
	{
		String str(tst);
		String str2(str);

		String newstr = str.erase(1, 2);

		ASSERT_STREQ(newstr.data(), "tt");
		ASSERT_STREQ(str2.data(), "test");

		ASSERT_EQ(newstr.countRef(), 2);
		ASSERT_EQ(newstr.size(), 2);
		ASSERT_EQ(newstr.capacity(), 5);

		ASSERT_EQ(str2.countRef(), 1);
		ASSERT_EQ(str2.size(), 4);
		ASSERT_EQ(str2.capacity(), 5);
	}


	{
		String str(tst);
		String newstr = str.erase(1, 20);

		ASSERT_STREQ(newstr.data(), "t");
		ASSERT_EQ(newstr.countRef(), 2);
		ASSERT_EQ(newstr.size(), 1);
		ASSERT_EQ(newstr.capacity(), 5);
	}

	{
		String str(tst);
		String newstr = str.erase(1);

		ASSERT_STREQ(newstr.data(), "t");
		ASSERT_EQ(newstr.countRef(), 2);
		ASSERT_EQ(newstr.size(), 1);
		ASSERT_EQ(newstr.capacity(), 5);
	}

	{
		String str(tst);
		String newstr = str.erase();

		ASSERT_STREQ(newstr.data(), "");
		ASSERT_EQ(newstr.countRef(), 2);
		ASSERT_EQ(newstr.size(), 0);
		ASSERT_EQ(newstr.capacity(), 5);
	}

	{
		String str(tst);
		ASSERT_THROW(str.erase(100), std::out_of_range);
	}


	{
		String nth(nothing);
		nth.erase(0);
		ASSERT_EQ(nth.size(), 0);
		ASSERT_EQ(nth.capacity(), 1);
		ASSERT_EQ(nth.countRef(), 1);
		ASSERT_STREQ(nth.data(), "");
	}
}

TEST(TestFunc, Replace) {
	{
		String str(tst);
		String str2(TEST_STRING.c_str());
		str += tst;


		str.replace(2, 4, str2);

		ASSERT_STREQ(str.data(), "teTest stringst");
		ASSERT_STREQ(str2.data(), TEST_STRING.c_str());

		ASSERT_EQ(str.countRef(), 1);
		ASSERT_EQ(str.size(), 15);
		ASSERT_EQ(str.capacity(), 16); 

		ASSERT_EQ(str2.countRef(), 1);
		ASSERT_EQ(str2.size(), 11);
		ASSERT_EQ(str2.capacity(), 12);
	}

	{
		String str(tst);
		str += tst;

		str.replace(2, 4, tst);

		ASSERT_STREQ(str.data(), "tetestst");
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_EQ(str.size(), 8);
		ASSERT_EQ(str.capacity(), 9);
	}
	
	{
		String str(tst);
		str += tst;

		str.replace(2, 4, 4, 'e');

		ASSERT_STREQ(str.data(), "teeeeest");
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_EQ(str.size(), 8);
		ASSERT_EQ(str.capacity(), 9);
	}
}

TEST(TestFunc, ReplaceShorter) {
	{
		String str(tst);
		str += tst;

		str.replace(2, 4, "ee");

		ASSERT_STREQ(str.data(), "teeest");
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_EQ(str.size(), 6);
		ASSERT_EQ(str.capacity(), 9);
	}

	{
		String str(tst);
		str += tst;

		str.replace(2, 4, 1, 'e');

		ASSERT_STREQ(str.data(), "teest");
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_EQ(str.size(), 5);
		ASSERT_EQ(str.capacity(), 9);
	}
}

TEST(TestFunc, Swap) {
	{
		String str(tst);
		str += tst;
		String str2(tst);

		str.swap(str2);

		ASSERT_STREQ(str.data(), "test");
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_EQ(str.size(), 4);
		ASSERT_EQ(str.capacity(), 5);

		ASSERT_STREQ(str2.data(), "testtest");
		ASSERT_EQ(str2.countRef(), 1);
		ASSERT_EQ(str2.size(), 8);
		ASSERT_EQ(str2.capacity(), 9);
	}
}
TEST(TestFunc, Find) {
	String str("the best test");

	{
		String toFind1("best");
		String toFind2("test");
		String toFind3("q");

		ASSERT_EQ(str.find(toFind1), 4);
		ASSERT_EQ(str.find(toFind2), 9);
		ASSERT_EQ(str.find(toFind3), (size_t)(-1));
	}

	{
		ASSERT_EQ(str.find("best"), 4);
	}

	{
		ASSERT_EQ(str.find('e'), 2);
	}

	{
		String nth(nothing);
		ASSERT_EQ(nth.find(""), (size_t)(-1));
		ASSERT_EQ(nth.find("qwe"), (size_t)(-1));

	}
}

TEST(TestFunc, Substr) {

	{
		String str(tst);

		String str2 = str.substr();

		ASSERT_STREQ(str.data(), tst);
		ASSERT_EQ(str.countRef(), 2);
		ASSERT_STREQ(str2.data(), tst);
		ASSERT_EQ(str2.countRef(), 2);
	}

	{
		String str("the best test");
		String str2 = str.substr(4, 4);

		ASSERT_STREQ(str.data(), "the best test");
		ASSERT_EQ(str.countRef(), 1);
		ASSERT_STREQ(str2.data(), "best");
		ASSERT_EQ(str2.countRef(), 1);
	}

}

TEST(TestFunc, Compare) {
	{
		String a("aaa");
		String b("aab");
		String c("aac");

		ASSERT_EQ(b.compare(a), 1);
		ASSERT_EQ(b.compare(b), 0);
		ASSERT_EQ(b.compare(c), -1);
	} 
	
	{
		String b("aab");

		ASSERT_EQ(b.compare("aaa"), 1);
		ASSERT_EQ(b.compare("aab"), 0);
		ASSERT_EQ(b.compare("aac"), -1);
	}
}

//---------------------------------


TEST(Func, Clear_EmptyStrCopy) {
	String str;
	String copy(str);
	EXPECT_TRUE(copy.countRef() == 2);
	EXPECT_TRUE(str.countRef() == 2);
	copy.clear();
	ASSERT_TRUE(copy.countRef() == 2);
	ASSERT_TRUE(str.countRef() == 2);
}

TEST(Ctr, emptyStr) {
	{
		const String str("", 10);
		ASSERT_TRUE(str.size() == 0);
	}
}

TEST(Func, Size_CopyPosLenConstructor) {
	const String str(TEST_STRING.c_str());
	std::vector<String> vector;
	for (size_t i = 0; i < 100; ++i) {
		vector.emplace_back(str, TEST_SIZE, TEST_SIZE); // copy constructor BOOM
		ASSERT_TRUE(vector.back().size() == 0);
	}
}

TEST(Func, Reserve_LessThanCapacity) {
	String str(TEST_STRING.c_str());
	const auto capacity = str.capacity();
	const auto data = str.data();

	str.reserve(capacity - 1);
	ASSERT_TRUE(capacity == str.capacity());
	ASSERT_TRUE(data == str.data());// BOOM

	str.reserve(capacity);
	ASSERT_TRUE(capacity == str.capacity());
	ASSERT_TRUE(data == str.data());
}
TEST(Func, Clear_CheckCapacity) {

	String str(10000, 'c');

	const auto capacity = str.capacity();

	str.clear();

	ASSERT_TRUE(capacity == str.capacity()); // ÂÇÐÛÂÀÅÒÑß

}

TEST(Func, Clear_CheckCapacityCopy) {

	String str(10000, 'c');

	String copy(str);

	const auto capacity = str.capacity();

	str.clear();

	ASSERT_TRUE(capacity != str.capacity()); // ÎÊ

}

TEST(Func, ConstAt_NoExceptionEmptyString) {

	{

		const String str;

		ASSERT_EQ(str.at(0), '\0');

	}

	{

		const String str(nullptr);

		ASSERT_EQ(str.at(0), '\0');

	}

	{

		const String str(nullptr, String::npos);

		ASSERT_EQ(str.at(0), '\0');

	}

	{

		const String str("\0");

		ASSERT_EQ(str.at(0), '\0');

	}

	{

		const String str(TEST_STRING.c_str(), 0);

		ASSERT_EQ(str.at(0), '\0');

	}

	{

		const String str(TEST_STRING.c_str());

		const String copy(str, TEST_SIZE);

		ASSERT_EQ(copy.at(0), '\0');

	}

}

TEST(Func, NonConstAt_ExceptionEmptyString) {
	{
		String str;
		ASSERT_THROW(str.at(0), std::out_of_range);
	}
	{
		String str(TEST_STRING.c_str());
		String copy(str, TEST_SIZE);
		ASSERT_THROW(copy.at(0), std::out_of_range);
	}
}

TEST(Func, OperatorPlusAssignment_PlusNullTerminatedChar)
{
	String str;
	str += '\0';
	ASSERT_TRUE(!str.empty());
	ASSERT_TRUE(1 == str.size());
}

TEST(Func, OperatorPlusAssignment_PlusEmptyStringForCopy)
{
	String str(TEST_STRING.c_str());
	const String copy(str);
	str += nullptr;
	ASSERT_TRUE(copy.countRef() == 2);
	str += "";
	ASSERT_TRUE(copy.countRef() == 2);
	str += "\0";
	ASSERT_TRUE(copy.countRef() == 2);
}

TEST(Func, Insert_EmptyStringsForCopy) {
	String emptyString;
	String str(TEST_STRING.c_str());
	const String copy(str);
	str.insert(0, nullptr);
	ASSERT_TRUE(copy.countRef() == 2);
	str.insert(0, "");
	ASSERT_TRUE(copy.countRef() == 2);
	str.insert(0, "\0");
	ASSERT_TRUE(copy.countRef() == 2);
	str.insert(0, 0, '\0');
	ASSERT_TRUE(copy.countRef() == 2);
	str.insert(0, emptyString);
	ASSERT_TRUE(copy.countRef() == 2);
}


TEST(Func, Replace_Exception) {

	{

		String str;

		ASSERT_THROW(str.replace(1, 1, "0"), std::out_of_range); // âîò çäåñü - ÏÐÎÕÎÄÈÒ

	}

	{

		String str(TEST_STRING.c_str());

		ASSERT_THROW(str.replace(TEST_SIZE + 1, 1, String()), std::out_of_range); // à âîò çäåñü – ïî÷åìó-òî, ÍÅ ÏÐÎÕÎÄÈÒ

	}

	{

		String str(TEST_STRING.c_str());

		ASSERT_THROW(str.replace(TEST_SIZE + 1, 1, 1, 'c'), std::out_of_range); // ?? äî ñþäà íå äîõîäèò…

	}

}


TEST(Func, Insert_ValidShortÑStrings) {

	String str(TEST_STRING.c_str());

	str.reserve(TEST_SIZE + 5);

	std::string expected = TEST_STRING;

	const char* strToInsert = "ab";



	for (size_t i = 0; i < 5; ++i) {

		str.insert(i, strToInsert);

		expected.insert(i, strToInsert);

		EXPECT_STREQ(expected.c_str(), str.data());

	}

}