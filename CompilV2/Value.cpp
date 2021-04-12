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
	return to_string(_val);
}

int IntValue::GetValue() {
	return _val;
}


DoubleValue::DoubleValue(double val) :Value(ValueType::FLOAT) {
	_val = val;
}

string DoubleValue::ToString() {
	return to_string(_val);
}

double DoubleValue::GetValue() {
	return _val;
}


CharValue::CharValue(char val) :Value(ValueType::CHARACTER) {
	_val = val;
}

string CharValue::ToString() {
	return string(1, _val);
}

char CharValue::GetValue() {
	return _val;
}


StringValue::StringValue(string val) :Value(ValueType::STRING) {
	_val = val;
}

string StringValue::ToString() {
	return _val;
}

string StringValue::GetValue() {
	return _val;
}