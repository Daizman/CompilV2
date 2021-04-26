#pragma once
#include "SemIdentificator.h"
#include "Lexer.hpp"

class Scope {
private:
	vector<ValueType> _types; //““
	vector<string> _names;
	ScopeType _type;
	Scope* _parScope;
	string _activeVarName = "";
	vector<Error> _errors;
	map<string, SemIdentificator*> _idMap; //“»
	IOModule* _ioModule;
	int CheckScopeTypeVec(vector<ScopeType>, ScopeType);
	SemIdentificator* CheckTypeByName(string, vector<ScopeType>);
	SemIdentificator* CheckName(string);
	string StrType(ValueType);
	int StrVecChecker(vector<string>, string);
public:
	Scope(Scope*, IOModule*, ScopeType);
	Scope();
	~Scope();
	void InitMainScpoe(IOModule*);
	void ClearTypes();
	void ClearNames();
	void ClearScopeIfErrors();
	void Clear();
	void RaiseError(int, int, string, int);
	void AddType(string);
	void AddName(string);
	void AddConst(string, Value*);
	void CheckInitVarTypeError(ValueType);
	void CheckIdentScope(string);
	void AddVars();
	void AddNone();
	void SetPar(Scope*);
	void SetScopeType(ScopeType);
	ScopeType GetScopeType();
	vector<string> GetNames();
	vector<ValueType> GetTypes();
	void PrintErrors();
};

