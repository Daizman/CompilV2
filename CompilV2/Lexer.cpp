#pragma once
#include "Lexer.hpp"


Lexer::Lexer(string iPath, string oPath) {
	_ioModule = new IOModule(iPath, oPath);
	_curToken = NULL;
    _symbMap["*"] = OperatorType::STAR;
    _symbMap["/"] = OperatorType::SLASH;
    _symbMap["="] = OperatorType::EQUAL;
    _symbMap[","] = OperatorType::COMMA;
    _symbMap[";"] = OperatorType::SEMICOLON;
    _symbMap[":"] = OperatorType::COLON;
    _symbMap["."] = OperatorType::POINT;
    _symbMap["^"] = OperatorType::ARROW;
    _symbMap["("] = OperatorType::LEFTPAR;
    _symbMap[")"] = OperatorType::RIGHTPAR;
    _symbMap["["] = OperatorType::LBRACKET;
    _symbMap["]"] = OperatorType::RBRACKET;
    _symbMap["{"] = OperatorType::FLPAR;
    _symbMap["}"] = OperatorType::FRPAR;
    _symbMap["<"] = OperatorType::LATER;
    _symbMap[">"] = OperatorType::GREATER;
    _symbMap["<="] = OperatorType::LATEREQUAL;
    _symbMap[">="] = OperatorType::GREATEREQUAL;
    _symbMap["<>"] = OperatorType::LATERGREATER;
    _symbMap["+"] = OperatorType::PLUS;
    _symbMap["-"] = OperatorType::MINUS;
    _symbMap["(*"] = OperatorType::LCOMMENT;
    _symbMap["*)"] = OperatorType::RCOMMENT;
    _symbMap[":="] = OperatorType::ASSIGN;
    _symbMap[".."] = OperatorType::TWOPOINTS;
    _symbMap["ident"] = OperatorType::IDENT;
    _symbMap["intconst"] = OperatorType::INTC;
    _symbMap["floatconst"] = OperatorType::FLOATC;
    _symbMap["charconst"] = OperatorType::CHARC;
    _symbMap["stringconst"] = OperatorType::STRINGC;
    _symbMap["case"] = OperatorType::CASESY;
    _symbMap["else"] = OperatorType::ELSESY;
    _symbMap["goto"] = OperatorType::GOTOSY;
    _symbMap["type"] = OperatorType::TYPESY;
    _symbMap["with"] = OperatorType::WITHSY;
    _symbMap["file"] = OperatorType::FILESY;
    _symbMap["then"] = OperatorType::THENSY;
    _symbMap["until"] = OperatorType::UNTILSY;
    _symbMap["do"] = OperatorType::DOSY;
    _symbMap["if"] = OperatorType::IFSY;
}

Lexer::~Lexer() {
	delete _ioModule;
	delete _curToken;
}

Token* Lexer::GetNextToken() {
	char symb = _ioModule->GetCurSymb();
    int curStrNum, curStrPos;

	if (symb == '\0') {
		return NULL;
	}
    if (symb == ' ') {
        _ioModule->SkipSpaces();
        symb = _ioModule->GetCurSymb();
    }
    vector<char> word;
    word.push_back(symb);

	if (IsCharacter(symb)) {
        char futureChar = _ioModule->CheckNextRange(1)[0];
		while ((IsCharacter(futureChar) || IsDigit(futureChar)) && symb != '\0') {
			symb = _ioModule->GetNextSymb();
			word.push_back(symb);
            futureChar = _ioModule->CheckNextRange(1)[0];
		}
        
		string wordStr(word.begin(), word.end());
        auto it = _symbMap.find(wordStr);
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
		if (it != _symbMap.end()) {
			auto sToken = new Operator(wordStr, _symbMap[wordStr]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
		} else {
            auto iToken = new Identificator(wordStr);
            _curToken = new IdentificatorToken(iToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
		}
	} else if (IsDigit(symb)) {
        bool haveDot = false;
        while (IsDigit(symb) || (symb == '.' && !haveDot)) {
            symb = _ioModule->GetNextSymb();
            word.push_back(symb);
            if (symb == '.') {
                haveDot = true;
            }
        }
        string wordStr(word.begin(), word.end());
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        if (haveDot) {
            double dVal = stod(wordStr);
            auto vdToken = new DoubleValue(dVal);
            _curToken = new ValueToken(vdToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
        } else {
            int iVal = stoi(wordStr);
            auto viToken = new IntValue(iVal);
            _curToken = new ValueToken(viToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
        }
	} else if (symb == '\'') {
        symb = _ioModule->GetNextSymb();
        word.push_back(symb);
        while (symb != '\'' && symb != '\0') {
            symb = _ioModule->GetNextSymb();
            word.push_back(symb);
        }
        if (symb == '\'') {
            curStrNum = _ioModule->GetCurStringNum();
            curStrPos = _ioModule->GetCurSymbNum();
            if (word.size() == 3) {
                auto vcToken = new CharValue(word[1]);
                _curToken = new ValueToken(vcToken, curStrNum, curStrPos - 3, curStrPos);
            } else {
                string wordStr(word.begin() + 1, word.end() - 1);
                auto vsToken = new StringValue(wordStr);
                _curToken = new ValueToken(vsToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
            }
        } else {
            
        }
    } else {
        string str = string(1, symb);
        auto it = _symbMap.find(str);
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        if (it != _symbMap.end()) {
            auto sToken = new Operator(str, _symbMap[str]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
        } else {
            
        }
	}
    _ioModule->GetNextSymb();
    return _curToken;
}

Token* Lexer::GetCurToken() {
	return _curToken;
}

Error* Lexer::RaiseError(int code, Token* problemToken) {
	return NULL;
}

bool Lexer::IsCharacter(char ch) {
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

bool Lexer::IsDigit(char ch) {
	return '0' <= ch && ch <= '9';
}