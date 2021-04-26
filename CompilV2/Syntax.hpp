#pragma once
#include "Lexer.hpp"


// границы работы: 
// фиктивный scope: зарезервированные значение: true, false
// нефиктивный - то что написал пользователь
// типы - переменные, которые можно переопределять!!!
// синтаксис после лексера
// семантика после синтаксиса (параллельно)
// лексер только за вынос токена

// ПРОПУСК ОШИБОК:
// для каждой конструкции БНФ, одна большая для программы
// для каждой БНФ своя функция
// ошибки пропускаем глобально
// для каждой функции будет список слов, которые могут идти после
// пропускаю до след. слова или конца программы (var - begin, например)
// это передаем


class Syntax {
private:
	Lexer* _lexer;
	IOModule* _ioModule;
	Token* _curToken;
	vector<IdentificatorToken*> _idents;
	vector<Error> _errors;
	string _progName;
	bool CheckLexem(string);
	bool CheckLexemWS(string);
	void NextToken();
	int StrVecChecker(vector<string>, string);
	bool GetProgName();
	bool EmptyToken();
	bool SkipToOper(string);
	bool SkipToIdent(string);
	bool SkipToOneOfOpers(vector<string>);
	bool SkipToOneOfIdents(vector<string>);
	bool IsAdditive();
	bool IsMultipl();
	bool CheckIdentWS(string);
	bool CheckIdent(string);
public:
	Syntax(string, string);
	~Syntax();
	Lexer* GetLexer();
	void PrintErrors();
	void RaiseError(int, int, string, int);
	void BNFProg();
	void BNFBlock();
	void BNFConsts();
	void BNFVariants();
	void BNFOpers();
	void BNFConstDif();
	void BNFSingleTypeVariants();
	void BNFConcOper();
	void BNFOper();
	void BNFNotMarkedOper();
	void BNFSimpleOper();
	void BNFOperAssigm();
	void BNFVar();
	void BNFFullVar();
	void BNFVarName();
	void BNFExpres();
	void BNFSimpleExpr();
	void BNFDiffExpr();
	void BNFAdditOper();
	void BNFSlag();
	void BNFMultOper();
	void BNFMultiplier();
};

