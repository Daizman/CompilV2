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
	if (CheckScopeTypeVec(ficScope, _idMap[name]->GetScope()) != -1) {
		return _idMap[name];
	}
	if (_parScope != NULL) {
		return _parScope->CheckTypeByName(name, ficScope);
	}
	return NULL;
}

SemIdentificator* Scope::CheckName(string name) {
	if (_idMap.find(name) == _idMap.end()) {
		return NULL;
	}
	if (_idMap[name]->GetScope() == _type) {
		return _idMap[name];
	}
	if (_parScope != NULL) {
		return _parScope->CheckName(name);
	}
	return NULL;
}

string Scope::StrType(ValueType vt) {
	switch (vt) {
	case ValueType::INTEGER:
		return "integer";
		break;
	case ValueType::FLOAT:
		return "real";
		break;
	case ValueType::CHARACTER:
		return "char";
		break;
	case ValueType::STRING:
		return "string";
		break;
	default:
		return "none";
		break;
	}
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
	_idMap.insert({ "integer", new SemIdentificator("integer", ScopeType::TYPES, ValueType::INTEGER) });

	_types.push_back(ValueType::FLOAT);
	_idMap.insert({ "real", new SemIdentificator("real", ScopeType::TYPES, ValueType::FLOAT) });

	_types.push_back(ValueType::STRING);
	_idMap.insert({ "string", new SemIdentificator("string", ScopeType::TYPES, ValueType::STRING) });

	_types.push_back(ValueType::CHARACTER);
	_idMap.insert({ "char", new SemIdentificator("char", ScopeType::TYPES, ValueType::CHARACTER) });
}

void Scope::ClearTypes() {
	_types.clear();
}

void Scope::ClearNames() {
	_names.clear();
}

void Scope::ClearScopeIfErrors() {
	if (_errors.size() != 0) {
		Clear();
	}
}

void Scope::Clear() {
	ClearTypes();
	ClearNames();
	_idMap.clear();
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
	auto n = CheckName(name);
	if(n != NULL) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Добавление существующего имени", 73);
		return;
	}
	_names.push_back(name);
}

void Scope::AddConst(string name, Value* val) {
	_types.push_back(val->GetType());
	_idMap.insert({ name, new SemIdentificator(StrType(val->GetType()), ScopeType::CONSTS, val->GetType()) });
}

void Scope::AddVars() {
	for (auto n : _names) {
		_idMap.insert({n, new SemIdentificator(StrType(GetTypes()[0]), ScopeType::VARS, GetTypes()[0])});
	}
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
