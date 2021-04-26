#pragma once
#include "IOModule.hpp"


char IOModule::GetNextSymb() {
	if (_curSymbNum >= _curString.length() - 1) {
		if (_input.eof()) {
			_isEnd = true;
			_curSymb = '\0';
			return _curSymb;
		}
		getline(_input, _curString);
		_curStringNum++;
		_curSymbNum = 0;
		Skip();
	} else {
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
	if (_curSymbNum + range + 1 >= _curString.length()) {
		range = _curString.length() - _curSymbNum - 1;
	}
	return _curString.substr(_curSymbNum + 1, range);
}

char IOModule::CheckNext() {
	if (_curSymbNum + 1 >= _curString.length()) {
		return '\n';
	}
	return _curString[_curSymbNum + 1];
}

void IOModule::WriteNext() {

}

void IOModule::PrintErrors(vector<Error> errors) {
	for (auto er : errors) {
		cout << er.ToString() << endl;
	}
}

void IOModule::SkipSymb() {
	_curSymbNum++;
	_curSymb = _curString[_curSymbNum];
}

void IOModule::Skip() {
	string symbToSkip = "\t\n﻿ ";
	int curStringLen = _curString.length();
	bool thisIsComment = curStringLen > _curSymbNum + 2 && _curString[_curSymbNum] == '/' && _curString[_curSymbNum + 1] == '/';
	while (!_input.eof() && thisIsComment) {
		while (_curSymbNum < curStringLen) {
			_curSymbNum++;
		}
		getline(_input, _curString);
		_curStringNum++;
		_curSymbNum = 0;
		curStringLen = _curString.length();
		thisIsComment = curStringLen > _curSymbNum + 2 && _curString[_curSymbNum] == '/' && _curString[_curSymbNum + 1] == '/';
	}

	while (!_input.eof() && (_curSymbNum < curStringLen && symbToSkip.find(_curString[_curSymbNum]) != string::npos || thisIsComment)) {
		_curSymbNum++;
		if (curStringLen == _curSymbNum) {
			getline(_input, _curString);
			_curStringNum++;
			_curSymbNum = 0;
			curStringLen = _curString.length();
		}
		thisIsComment = curStringLen > _curSymbNum + 2 && _curString[_curSymbNum] == '/' && _curString[_curSymbNum + 1] == '/';
		while (!_input.eof() && thisIsComment) {
			while (_curSymbNum < curStringLen) {
				_curSymbNum++;
			}
			getline(_input, _curString);
			_curStringNum++;
			_curSymbNum = 0;
			curStringLen = _curString.length();
			thisIsComment = curStringLen > _curSymbNum + 2 && _curString[_curSymbNum] == '/' && _curString[_curSymbNum + 1] == '/';
		}
	}
	if (_input.eof() && _curString[_curSymbNum] == '\0') {
		_isEnd = true;
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
	getline(_input, _curString);
	Skip();
	_curSymb = _curString[_curSymbNum];
}

IOModule::~IOModule() {
	CloseInput();
	CloseOutput();
}
