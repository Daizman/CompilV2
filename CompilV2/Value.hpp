#pragma once
#pragma once
#include "ValueType.hpp"
#include <string>


using std::string;
using std::to_string;


class Value {
private:
	ValueType _type;
	string _name;
public:
	ValueType GetType();
	virtual string ToString() = 0;
	Value(ValueType, string);
	~Value();
};

class IntIdentificator : public Value {
private:
	int _val;
public:
	string ToString() override;
	int GetValue();
	IntIdentificator(int, string);
};

class FloatIdentificator : public Value {
private:
	float _val;
public:
	string ToString() override;
	float GetValue();
	FloatIdentificator(float, string);
};

class CharIdentificator : public Value {
private:
	char _val;
public:
	string ToString() override;
	char GetValue();
	CharIdentificator(char, string);
};

class StringIdentificator : public Value {
private:
	string _val;
public:
	string ToString() override;
	string GetValue();
	StringIdentificator(string, string);
};

