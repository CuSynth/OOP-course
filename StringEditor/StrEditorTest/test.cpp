 #include "gtest/gtest.h"
#include <sstream>
#include <fstream>

#include "../StringEditor/StrEditor.hpp"

using namespace std;

//const string dataFile("data.txt");
//const string commandFile("commands.txt");

const string cpCommand("copy 0, 21\npaste 0");
const string insertCommand("insert \"Prestring. \", 0");
const string deleteCommand("delete 0, 21");


const string strToEdit("Here is data string. Just for test.");	

const string EditAllCmd("delete 0, 35\n"
						"insert \"This is new string! \", 0\n"
						"insert \"And another new string!\", 20\n");

const string fullCommandsStr("				 copy				0,			20				\n"
	"insert					\" \",			 0				   \n"
	"paste		 0\n"
	"insert \" \", 0\n"
	"paste 0\n"
	"insert \" \", 0\n"
	"paste		0\n"
	"undo  \n"
	"undo		\n"
	"undo	\n"
	"undo \n"
	"	redo\n"
	"			 redo\n"
	"delete				 0,			42\n"
	"undo\n"
	"redo		 \n"
	"	copy 0, 35\n"
	"				insert			   \" \", 0\n"
	"paste				0");

//------------

const string errorCmd("redooo..");
const string errorArg("copy 0, 21\npaste -10");
const string notEnoughArgs("copy 0\npaste 10");
const string tooMuchArgs("copy 0, 1, 2, 3\npaste 10");


TEST(_parser, errCmd) {
	istringstream iss(strToEdit);
	istringstream cmdIss(errorCmd);

	auto str = make_shared<document>(iss);
	parser prs(str, cmdIss);

	ASSERT_THROW(prs.parse(), invalid_argument);
}

TEST(_parser, errArg) {
	istringstream iss(strToEdit);
	istringstream cmdIss(errorArg);

	auto str = make_shared<document>(iss);
	parser prs(str, cmdIss);

	ASSERT_THROW(prs.parse(), invalid_argument);
}

TEST(_parser, neArgs) {
	istringstream iss(strToEdit);
	istringstream cmdIss(notEnoughArgs);

	auto str = make_shared<document>(iss);
	parser prs(str, cmdIss);

	ASSERT_THROW(prs.parse(), invalid_argument);
}

TEST(_parser, tmArg) {
	istringstream iss(strToEdit);
	istringstream cmdIss(tooMuchArgs);

	auto str = make_shared<document>(iss);
	parser prs(str, cmdIss);

	ASSERT_THROW(prs.parse(), invalid_argument);
}


TEST(str_editor, errCmd) {
	istringstream iss(strToEdit);
	istringstream cmdIss(errorCmd);
	ostringstream err_oss;

	StrEditor _editor(iss, cmdIss, err_oss);
	ASSERT_NO_THROW(_editor.edit());
	ASSERT_TRUE(err_oss.str().size() != 0);
}

TEST(str_editor, errArg) {
	istringstream iss(strToEdit);
	istringstream cmdIss(errorArg);
	ostringstream err_oss;

	StrEditor _editor(iss, cmdIss, err_oss);
	ASSERT_NO_THROW(_editor.edit());
	ASSERT_TRUE(err_oss.str().size() != 0);
}

TEST(str_editor, neArg) {
	istringstream iss(strToEdit);
	istringstream cmdIss(tooMuchArgs);
	ostringstream err_oss;

	StrEditor _editor(iss, cmdIss, err_oss);
	ASSERT_NO_THROW(_editor.edit());
	ASSERT_TRUE(err_oss.str().size() != 0);
}

TEST(str_editor, tmArg) {
	istringstream iss(strToEdit);
	istringstream cmdIss(tooMuchArgs);
	ostringstream err_oss;

	StrEditor _editor(iss, cmdIss, err_oss);
	ASSERT_NO_THROW(_editor.edit());
	ASSERT_TRUE(err_oss.str().size() != 0);
}

//------------





TEST(_document, ctr) {
	istringstream iss(strToEdit);
	ostringstream oss;
	document str(iss);

	str.write(oss);
	ASSERT_EQ(oss.str(), "Here is data string. Just for test.");
}

TEST(_editor, ctr) {
	istringstream iss(strToEdit);
	auto str = make_shared<document>(iss);
	editor edt(str);

	ostringstream oss;
	edt.write(oss);
	ASSERT_EQ(oss.str(), "Here is data string. Just for test.");
}

TEST(_editor, raw_copy_paste) {
	istringstream iss(strToEdit);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);

	edt.apply(make_shared<copy_cmd>(vector<string>{"copy", "0", "20"}, str));
	edt.apply(make_shared<paste_cmd>(vector<string>{"paste", "0"}, str));
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Here is data string.Here is data string. Just for test.");
}

TEST(_editor, raw_insert) {
	istringstream iss(strToEdit);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);

	edt.apply(make_shared<insert_cmd>(vector<string>{"insert", "\"_Let it be here!_\"", "5"}, str));
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Here _Let it be here!_is data string. Just for test.");
}

TEST(_editor, raw_delete) {
	istringstream iss(strToEdit);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);

	edt.apply(make_shared<delete_cmd>(vector<string>{"delete", "0", "21"}, str));
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Just for test.");
}

TEST(parser_editor, copy_paste) {
	istringstream iss(strToEdit);
	istringstream cmdIss(cpCommand);
	ostringstream oss;
	
	auto str = make_shared<document>(iss);
	editor edt(str);
	parser prs(str, cmdIss);

	prs.parse();
	edt.apply(prs.getCommands());
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Here is data string. Here is data string. Just for test.");
}

TEST(parser_editor, cp_undo_redo) {
	istringstream iss(strToEdit);
	istringstream cmdIss(cpCommand);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);
	parser prs(str, cmdIss);

	prs.parse();
	edt.apply(prs.getCommands());
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Here is data string. Here is data string. Just for test.");

	edt.apply(make_shared<undo_cmd>());
	oss.str("");
	oss.clear();
	edt.write(oss);
	ASSERT_EQ(oss.str(), "Here is data string. Just for test.");

	edt.apply(make_shared<redo_cmd>());
	oss.str("");
	oss.clear();
	edt.write(oss);
	ASSERT_EQ(oss.str(), "Here is data string. Here is data string. Just for test.");
}

TEST(parser_editor, insert) {
	istringstream iss(strToEdit);
	istringstream cmdIss(insertCommand);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);
	parser prs(str, cmdIss);

	prs.parse();
	edt.apply(prs.getCommands());
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Prestring. Here is data string. Just for test.");
}

TEST(parser_editor, i_undo_redo) {
	istringstream iss(strToEdit);
	istringstream cmdIss(insertCommand);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);
	parser prs(str, cmdIss);

	prs.parse();
	edt.apply(prs.getCommands());
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Prestring. Here is data string. Just for test.");

	edt.apply(make_shared<undo_cmd>());
	oss.str("");
	oss.clear();
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Here is data string. Just for test.");

	edt.apply(make_shared<redo_cmd>());
	oss.str("");
	oss.clear();
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Prestring. Here is data string. Just for test.");
}

TEST(parser_editor, del) {
	istringstream iss(strToEdit);
	istringstream cmdIss(deleteCommand);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);
	parser prs(str, cmdIss);

	prs.parse();
	edt.apply(prs.getCommands());
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Just for test.");
}

TEST(parser_editor, d_undo_redo) {
	istringstream iss(strToEdit);
	istringstream cmdIss(deleteCommand);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);
	parser prs(str, cmdIss);

	prs.parse();
	edt.apply(prs.getCommands());
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Just for test.");

	edt.apply(make_shared<undo_cmd>());
	oss.str("");
	oss.clear();
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Here is data string. Just for test.");

	edt.apply(make_shared<redo_cmd>());
	oss.str("");
	oss.clear();
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Just for test.");
}

TEST(parser_editor, full) {
	istringstream iss(strToEdit);
	istringstream cmdIss(fullCommandsStr);
	ostringstream oss;

	auto str = make_shared<document>(iss);
	editor edt(str);
	parser prs(str, cmdIss);

	prs.parse();
	edt.apply(prs.getCommands());
	edt.write(oss);

	ASSERT_EQ(oss.str(), "Here is data string. Just for test. Here is data string. Just for test.");
}

TEST(str_editor, full) {
	istringstream iss(strToEdit);
	istringstream cmdIss(fullCommandsStr);
	ostringstream oss;
	ostringstream err_oss;

	StrEditor _editor(iss, cmdIss, err_oss);
	_editor.edit();
	_editor.write(oss);
	
	ASSERT_EQ(oss.str(), "Here is data string. Just for test. Here is data string. Just for test.");
}

TEST(str_editor, edit_all) {
	istringstream iss(strToEdit);
	istringstream cmdIss(EditAllCmd);
	ostringstream oss;
	ostringstream err_oss;

	StrEditor _editor(iss, cmdIss, err_oss);
	_editor.edit();
	_editor.write(oss);

	ASSERT_EQ(oss.str(), "This is new string! And another new string!");
}
