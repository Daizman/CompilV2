#pragma once
#include "Value.hpp"


Value::Value(ValueType itype) {
	_type = itype;
}

Value::~Value() {
	
}

ValueType Value::GetType() {
	return _type;
}


IntValue::IntValue(int val) :Value(ValueType::INTEGER) {
	_val = val;
}

string IntValue::ToString() {
	return "Value: " + to_string(_val) + "\n";
}

int IntValue::GetValue() {
	return _val;
}


DoubleValue::DoubleValue(double val) :Value(ValueType::FLOAT) {
	_val = val;
}

string DoubleValue::ToString() {
	return "Value: " + to_string(_val) + "\n";
}

double DoubleValue::GetValue() {
	return _val;
}


CharValue::CharValue(char val) :Value(ValueType::CHARACTER) {
	_val = val;
}

string CharValue::ToString() {
	return "Value: '" + string(1, _val) + "'\n";
}

char CharValue::GetValue() {
	return _val;
}


StringValue::StringValue(string val) :Value(ValueType::STRING) {
	_val = val;
}

string StringValue::ToString() {
	return "Value: '" + _val + "'\n";
}

string StringValue::GetValue() {
	return _val;
}