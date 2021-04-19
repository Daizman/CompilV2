#pragma once
#include "IOModule.hpp"
#include <map>
#include "OperatorType.hpp"

using std::map;
using std::stod;
using std::stoi;
using std::atof;


class Lexer {
private:
	IOModule* _ioModule;
	Token* _curToken;
	vector<Error> _errors;
	map<string, OperatorType> _symbMap;
	bool IsCharacter(char);
	bool IsDigit(char);
public:
	Lexer(string, string);
	~Lexer();
	Token* GetNextToken();
	Token* GetCurToken();
	void PrintErrors();
	void RaiseError(int, int, string, int);
};

