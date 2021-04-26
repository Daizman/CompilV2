#pragma once
#include "ValueType.hpp"
#include "ScopeType.hpp"
#include <string>

using std::string;


class SemIdentificator {
private:
	string _name;
	ScopeType _parScope;
	ValueType _type;
public:
	SemIdentificator(string, ScopeType, ValueType);
	~SemIdentificator();
	string GetName();
	ScopeType GetScope();
	ValueType GetType();
};

