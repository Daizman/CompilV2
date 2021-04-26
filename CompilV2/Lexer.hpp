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
	void CharacterTokenHandler(char);
	void DigitTokenHandler(char);
	void StringTokenHandler(char);
	void ColonHandler(char);
	void MoreHandler(char);
	void LowerHandler(char);
	void LongCommHandler(char);
	void OtherSymbHandler(char);
	void RaiseError(int, int, string, int);
public:
	Lexer(IOModule*);
	~Lexer();
	Token* GetNextToken();
	Token* GetCurToken();
	void PrintErrors();
};

