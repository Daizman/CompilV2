#include "Operator.hpp"


Operator::Operator(string symb, OperatorType type) {
    _symb = symb;
	_type = type;
}


Operator::~Operator() {
	
}

string Operator::GetSymb() {
	return _symb;
}

string Operator::ToString() {
    return "Operator: " + _symb + "\n";
}