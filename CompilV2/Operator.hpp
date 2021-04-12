#pragma once
#include "OperatorType.hpp"
#include <string>
#include <map>

using std::string;
using std::map;


class Operator {
private:
	OperatorType _type;
	string _symb;
public:
	Operator(string, OperatorType);
	~Operator();
	string GetSymb();
	string ToString();
};

