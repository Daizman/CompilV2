#pragma once
#include "IOModule.hpp"


char IOModule::GetNextSymb() {
	if (_input.eof()) {
		_isEnd = true;
		return '\0';
	}
	if (_curSymbNum >= _curString.length() || _curStringNum == 0) {
		SkipSpaces();
		if (_input.eof()) {
			_isEnd = true;
			return '\0';
		}
		_curSymbNum = 0;
	}
	else {
		_curSymbNum++;
	}
	_curSymb = _curString[_curSymbNum];

	return _curSymb;
}

int IOModule::GetCurStringNum() {
	return _curStringNum;
}

int IOModule::GetCurSymbNum() {
	return _curSymbNum;
}

char IOModule::GetCurSymb() {
	return _curSymb;
}

string IOModule::GetCurString() {
	return _curString;
}

bool IOModule::IsEnd() {
	return _isEnd;
}

string IOModule::CheckNextRange(int range) {
	if (_curSymbNum + range >= _curString.length()) {
		range = _curString.length() - _curSymbNum;
	}
	return _curString.substr(_curStringNum, range);
}

void IOModule::WriteNext() {

}

void IOModule::PrintListing() {

}

void IOModule::RaiseError(Error error) {

}

void IOModule::SkipSymb() {
	_curSymbNum++;
}

void IOModule::SkipSpaces() {
	string templateStr = "\t\n ";
	while (!_input.eof() && _curSymbNum < _curString.length() && templateStr.find(_curString[_curSymbNum]) != string::npos) {
		_curSymbNum++;
		if (_curString.length() == _curSymbNum) {
			getline(_input, _curString);
			_curStringNum++;
			_curSymbNum = 0;
		}
	}
}

void IOModule::OpenInput() {
	if (!_input.is_open()) {
		_input.open(_iPath);
	}
}

void IOModule::CloseInput() {
	if (_input.is_open()) {
		_input.close();
	}
}
void IOModule::OpenOutput() {
	if (!_output.is_open()) {
		_output.open(_oPath);
	}
}

void IOModule::CloseOutput() {
	if (_output.is_open()) {
		_output.close();
	}
}

IOModule::IOModule(string iPath, string oPath) {
	_iPath = iPath;
	_oPath = oPath;
	_isEnd = false;
	_curStringNum = 0;
	_curSymbNum = 0;
	OpenInput();
}

IOModule::~IOModule() {
	CloseInput();
	CloseOutput();
}
