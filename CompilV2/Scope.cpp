#include "Scope.h"

int Scope::CheckScopeTypeVec(vector<ScopeType> scope, ScopeType st) {
	int i = 0;
	for (auto s : scope) {
		if (s == st) {
			return i;
		}
		i++;
	}
	return -1;
}

SemIdentificator* Scope::CheckTypeByName(string name, vector<ScopeType> ficScope) {
	if (_idMap.find(name) == _idMap.end()) {
		return NULL;
	}
	if (CheckScopeTypeVec(ficScope, _idMap[name].GetScope()) != -1) {
		return &_idMap[name];
	}
	if (_parScope != NULL) {
		return _parScope->CheckTypeByName(name, ficScope);
	}
	return NULL;
}

Scope::Scope(Scope* parScope, IOModule* ioMod, ScopeType sType) {
	_parScope = parScope;
	_ioModule = ioMod;
	_type = sType;
}

Scope::Scope() {
}

Scope::~Scope() {
	delete _ioModule;
}

void Scope::InitMainScpoe(IOModule* ioMod) {
	_ioModule = ioMod;

	_type = ScopeType::MAIN;

	_types.push_back(ValueType::INTEGER);
	_idMap.insert({ "integer", SemIdentificator("integer", ScopeType::TYPES, ValueType::INTEGER) });

	_types.push_back(ValueType::FLOAT);
	_idMap.insert({ "real", SemIdentificator("real", ScopeType::TYPES, ValueType::FLOAT) });

	_types.push_back(ValueType::STRING);
	_idMap.insert({ "string", SemIdentificator("string", ScopeType::TYPES, ValueType::STRING) });

	_types.push_back(ValueType::CHARACTER);
	_idMap.insert({ "char", SemIdentificator("char", ScopeType::TYPES, ValueType::CHARACTER) });
}

void Scope::ClearTypes() {
	_types.clear();
}

void Scope::ClearNames() {
	_names.clear();
}

void Scope::Clear() {
	ClearTypes();
	ClearNames();
}

void Scope::RaiseError(int strNum, int pos, string reason, int code) {
	auto error = Error(strNum + 1, pos + 1, reason, code);
	_errors.push_back(error);
}

void Scope::AddType(string tp) {
	auto t = CheckTypeByName(tp, vector<ScopeType> {ScopeType::TYPES});
	if (t == NULL) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не верно указан тип", 70);
		return;
	}
	_types.push_back(t->GetType());
}

void Scope::AddName(string name) {
	_names.push_back(name);
}

void Scope::AddConst(ValueType t, string rp) {
	if (t == ValueType::NONE) {
		auto temp = CheckTypeByName(rp, vector<ScopeType> {ScopeType::CONSTS, ScopeType::VARS});
		if (temp == NULL) {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Переменная не найдена", 71);
			return;
		}
		_types.push_back(temp->GetType());
		return;
	}
	_types.push_back(t);
}

vector<string> Scope::GetNames() {
	return _names;
}

vector<ValueType> Scope::GetTypes() {
	return _types;
}

void Scope::PrintErrors() {
	_ioModule->PrintErrors(_errors);
}

void Scope::AddNone() {
	_types.push_back(ValueType::NONE);
}

void Scope::SetPar(Scope* par) {
	_parScope = par;
}

void Scope::SetScopeType(ScopeType st) {
	_type = st;
}

ScopeType Scope::GetScopeType() {
	return _type;
}
