#pragma once
#include "Error.hpp"

Error::Error(int strNum, int pos = 0, string reason = "", int code = 0) {
	_strNum = strNum;
	_pos = pos;
	_reason = reason;
	_code = code;
}

int Error::GetStrNum() {
	return _strNum;
}

int Error::GetPose() {
	return _pos;
}

string Error::GetReason() {
	return _reason;
}

int Error::GetCode() {
	return _code;
}

string Error::ToString() {
	string errorText = "";
	errorText += "Ошибка в строке " + to_string(_strNum);
	errorText += ", на позиции " + to_string(_pos);
	errorText += ", код ошибки " + to_string(_code);
	errorText += ", описание: " + _reason + ";";
	return errorText;
}