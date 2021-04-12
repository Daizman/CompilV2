#pragma once
#include "IOModule.hpp"
#include <map>
#include "OperatorType.hpp"

using std::map;
using std::stod;
using std::stoi;


class Lexer {
private:
	IOModule* _ioModule;
	Token* _curToken;
	map<string, OperatorType> _symbMap;
	bool IsCharacter(char);
	bool IsDigit(char);
public:
	Lexer(string, string);
	~Lexer();
	Token* GetNextToken();
	Token* GetCurToken();
	Error* RaiseError(int, Token*);
};

