#include "SemIdentificator.h"

SemIdentificator::SemIdentificator(string name, ScopeType parScope, ValueType siType) {
	_name = name;
	_parScope = parScope;
	_type = siType;
}

SemIdentificator::~SemIdentificator() {
}

string SemIdentificator::GetName() {
	return _name;
}

ScopeType SemIdentificator::GetScope() {
	return _parScope;
}

ValueType SemIdentificator::GetType() {
	return _type;
}
