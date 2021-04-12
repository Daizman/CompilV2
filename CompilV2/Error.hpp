#pragma once
#include <string>


using std::string;
using std::to_string;


class Error {
private:
	int _pos;
	int _strNum;
	string _reason;
	int _code;
public:
	Error(int, int, string, int);
	int GetStrNum();
	int GetPose();
	string GetReason();
	int GetCode();
	string ToString();
};

