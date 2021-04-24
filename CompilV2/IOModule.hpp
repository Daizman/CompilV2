#pragma once
#include "Token.hpp"
#include "Error.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


using std::string;
using std::vector;
using std::to_string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;


class IOModule {
private:
	string _iPath;
	string _oPath;
	ifstream _input;
	ofstream _output;
	int _curStringNum;
	int _curSymbNum;
	string _curString;
	char _curSymb;
	bool _isEnd;
	void Skip();
public:
	char GetNextSymb();
	string CheckNextRange(int);
	char CheckNext();
	void SkipSymb();
	void WriteNext();
	void OpenInput();
	void CloseInput();
	void OpenOutput();
	void CloseOutput();
	int GetCurStringNum();
	int GetCurSymbNum();
	string GetCurString();
	char GetCurSymb();
	bool IsEnd();
	void PrintErrors(vector<Error>);
	IOModule(string, string);
	~IOModule();
};
