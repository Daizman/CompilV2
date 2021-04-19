#pragma once
#include "Lexer.hpp"


Lexer::Lexer(IOModule* ioModule) {
    _ioModule = ioModule;
    if (_ioModule->GetCurSymb() == 'п') {
        _ioModule->SkipSymb();
        if (_ioModule->GetCurSymb() == '»') {
            _ioModule->SkipSymb();
            if (_ioModule->GetCurSymb() == 'ї') {
                _ioModule->SkipSymb();
            }
        }
    }
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
    char futureChar;

	if (symb == '\0' || _ioModule->IsEnd()) {
		return NULL;
	}
    if (symb == ' ') {
        _ioModule->SkipSpaces();
        symb = _ioModule->GetCurSymb();
    }
    vector<char> word;
    word.push_back(symb);

	if (IsCharacter(symb)) {
        futureChar = _ioModule->CheckNextRange(1)[0];
		while (!_ioModule->IsEnd() && (IsCharacter(futureChar) || IsDigit(futureChar)) && symb != '\0') {
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
        _ioModule->GetNextSymb();
	} else if (IsDigit(symb)) {
        bool haveDot = false;
        while (!_ioModule->IsEnd() && IsDigit(symb) || symb == '.') {
            symb = _ioModule->GetNextSymb();
            if (symb == '.') {
                if (!haveDot) {
                    haveDot = true;
                    word.push_back(',');
                } else {
                    curStrNum = _ioModule->GetCurStringNum();
                    curStrPos = _ioModule->GetCurSymbNum();
                    RaiseError(curStrPos, curStrNum, "Несколько точек в определении числа", 1);
                    return GetNextToken();
                }
            }
            if (IsDigit(symb)) {
                word.push_back(symb);
            }
        }
        string wordStr(word.begin(), word.end());
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        if (haveDot) {
            double dVal = std::stod(wordStr);
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
        int oldStrNum = _ioModule->GetCurStringNum();
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        while (!_ioModule->IsEnd() && symb != '\'' && symb != '\0' && oldStrNum == _ioModule->GetCurStringNum()) {
            symb = _ioModule->GetNextSymb();
            word.push_back(symb);
        }
        if (symb == '\'' && oldStrNum == _ioModule->GetCurStringNum()) {
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
            RaiseError(curStrPos, curStrNum, "Не закрыта кавычка у строковой константы", 2);
            return GetNextToken();
        }
        _ioModule->GetNextSymb();
    } else if (symb == ':') {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        futureChar = _ioModule->CheckNextRange(1)[0];
        if (futureChar == '=') {
            auto sToken = new Operator(":=", _symbMap[":="]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
            _ioModule->GetNextSymb();
        } else {
            string str = string(1, symb);
            auto it = _symbMap.find(str);
            if (it != _symbMap.end()) {
                auto sToken = new Operator(str, _symbMap[str]);
                _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
            } else {
                RaiseError(curStrPos, curStrNum, "Неопознанный символ: '" + str + "'", 3);
                _curToken = GetNextToken();
            }
        }
        _ioModule->GetNextSymb();
    } else if (symb == '/') {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        futureChar = _ioModule->CheckNextRange(1)[0];
        if (futureChar == '/') {
            int oldStrNum = _ioModule->GetCurStringNum();
            while (!_ioModule->IsEnd() && oldStrNum == _ioModule->GetCurStringNum()) {
                symb = _ioModule->GetNextSymb();
            }
            _curToken = GetNextToken();
        } else {
            auto sToken = new Operator("/", _symbMap["/"]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
            _ioModule->GetNextSymb();
        }
    } else if (symb == '{') {
        while (!_ioModule->IsEnd() && symb != '}') {
            symb = _ioModule->GetNextSymb();
        }
        if (_ioModule->IsEnd()) {
            curStrNum = _ioModule->GetCurStringNum();
            curStrPos = _ioModule->GetCurSymbNum();
            RaiseError(curStrPos, curStrNum, "Незакрытый комментарий", 4);
            return GetNextToken();
        } else {
            _ioModule->GetNextSymb();
            _curToken = GetNextToken();
        }
    } else if (symb == '<') {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        futureChar = _ioModule->CheckNextRange(1)[0];
        if (futureChar == '>') {
            auto sToken = new Operator("<>", _symbMap["<>"]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
            _ioModule->GetNextSymb();
        } else if (futureChar == '=') {
            auto sToken = new Operator("<=", _symbMap["<="]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
            _ioModule->GetNextSymb();
        } else {
            auto sToken = new Operator("<", _symbMap["<"]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
        }
        _ioModule->GetNextSymb();
    } else if (symb == '>') {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        futureChar = _ioModule->CheckNextRange(1)[0];
        if (futureChar == '=') {
            auto sToken = new Operator(">=", _symbMap[">="]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
            _ioModule->GetNextSymb();
        } else {
            auto sToken = new Operator(">", _symbMap[">"]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
        }
        _ioModule->GetNextSymb();
    } else if (symb == '-') {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        futureChar = _ioModule->CheckNextRange(1)[0];
        if (IsDigit(futureChar)) {
            bool haveDot = false;
            word.push_back(symb);
            symb = _ioModule->GetNextSymb();
            while (!_ioModule->IsEnd() && IsDigit(symb) || symb == '.') {
                symb = _ioModule->GetNextSymb();
                if (symb == '.') {
                    if (!haveDot) {
                        haveDot = true;
                        word.push_back(',');
                    } else {
                        curStrNum = _ioModule->GetCurStringNum();
                        curStrPos = _ioModule->GetCurSymbNum();
                        RaiseError(curStrPos, curStrNum, "Несколько точек в определении числа", 1);
                        return GetNextToken();
                    }
                }
                if (IsDigit(symb)) {
                    word.push_back(symb);
                }
            }
            string wordStr(word.begin(), word.end());
            curStrNum = _ioModule->GetCurStringNum();
            curStrPos = _ioModule->GetCurSymbNum();
            if (haveDot) {
                double dVal = std::stod(wordStr);
                auto vdToken = new DoubleValue(dVal);
                _curToken = new ValueToken(vdToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
            } else {
                int iVal = stoi(wordStr);
                auto viToken = new IntValue(iVal);
                _curToken = new ValueToken(viToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
            }
        } else {
            auto sToken = new Operator("-", _symbMap["-"]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
            _ioModule->GetNextSymb();
        }
    } else {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        
        string str = string(1, symb);
        auto it = _symbMap.find(str);
        if (it != _symbMap.end()) {
            auto sToken = new Operator(str, _symbMap[str]);
            _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
        } else {
            RaiseError(curStrPos, curStrNum, "Неопознанный символ: '" + str + "'", 3);
            _ioModule->GetNextSymb();
            return GetNextToken();
        }
        _ioModule->GetNextSymb();
	}

    return _curToken;
}

Token* Lexer::GetCurToken() {
	return _curToken;
}

void Lexer::RaiseError(int pos, int strNum, string reason, int code) {
    auto error = Error(strNum + 1, pos + 1, reason, code);
    _errors.push_back(error);
}

void Lexer::PrintErrors() {
    _ioModule->PrintErrors(_errors);
}

bool Lexer::IsCharacter(char ch) {
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

bool Lexer::IsDigit(char ch) {
	return '0' <= ch && ch <= '9';
}