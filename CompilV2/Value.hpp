#pragma once
#pragma once
#include "ValueType.hpp"
#include <string>


using std::string;
using std::to_string;


class Value {
private:
	ValueType _type;
public:
	ValueType GetType();
	virtual string ToString() = 0;
	Value(ValueType);
	~Value();
};

class IntValue : public Value {
private:
	int _val;
public:
	string ToString() override;
	int GetValue();
	IntValue(int);
};

class DoubleValue : public Value {
private:
	double _val;
public:
	string ToString() override;
	double GetValue();
	DoubleValue(double);
};

class CharValue : public Value {
private:
	char _val;
public:
	string ToString() override;
	char GetValue();
	CharValue(char);
};

class StringValue : public Value {
private:
	string _val;
public:
	string ToString() override;
	string GetValue();
	StringValue(string);
};

