#pragma once
#include "Lexer.hpp"


Lexer::Lexer(IOModule* ioModule) {
    _ioModule = ioModule;
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
    _symbMap["program"] = OperatorType::PROGRAM;
}

Lexer::~Lexer() {
	delete _ioModule;
	delete _curToken;
}

void Lexer::CharacterTokenHandler(char symb) {
    vector<char> word;
    char futureChar = _ioModule->CheckNext();

    word.push_back(symb);

    while (!_ioModule->IsEnd() && (IsCharacter(futureChar) || IsDigit(futureChar))) {
        symb = _ioModule->GetNextSymb();
        word.push_back(symb);
        futureChar = _ioModule->CheckNext();
    }

    string wordStr(word.begin(), word.end());
    auto it = _symbMap.find(wordStr);
    int curStrNum = _ioModule->GetCurStringNum();
    int curStrPos = _ioModule->GetCurSymbNum();
    if (it != _symbMap.end()) {
        auto sToken = new Operator(wordStr, _symbMap[wordStr]);
        _curToken = new OperatorToken(sToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
    } else {
        auto iToken = new Identificator(wordStr);
        _curToken = new IdentificatorToken(iToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
    }
}

void Lexer::DigitTokenHandler(char symb) {
    bool haveDot = false;
    int curStrNum, curStrPos;
    vector<char> word;
    char futureChar = _ioModule->CheckNext();

    if ((futureChar == '.' || !IsDigit(futureChar)) && symb == '-') {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        RaiseError(curStrPos, curStrNum, "Не правильное определение отрицательного", 5);
        return;
    }

    word.push_back(symb);

    while (!_ioModule->IsEnd() && (IsDigit(futureChar) || futureChar == '.')) {
        symb = _ioModule->GetNextSymb();
        if (symb == '.') {
            if (!haveDot) {
                haveDot = true;
                symb = ',';
            } else {
                curStrNum = _ioModule->GetCurStringNum();
                curStrPos = _ioModule->GetCurSymbNum();
                RaiseError(curStrPos, curStrNum, "Несколько точек в определении числа", 1);
                return;
            }
        }
        word.push_back(symb);
        futureChar = _ioModule->CheckNext();
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
}

void Lexer::StringTokenHandler(char symb) {
    vector<char> word;
    char futureChar = _ioModule->CheckNext();
    int curStrNum = _ioModule->GetCurStringNum(), curStrPos = _ioModule->GetCurSymbNum();

    while (!_ioModule->IsEnd() && futureChar != '\'' && curStrNum == _ioModule->GetCurStringNum()) {
        symb = _ioModule->GetNextSymb();
        word.push_back(symb);
        futureChar = _ioModule->CheckNext();
    }

    if (futureChar != '\'') {
        RaiseError(curStrPos, curStrNum, "Не закрыта кавычка у строковой константы", 2);
        return;
    }

    symb = _ioModule->GetNextSymb();

    curStrNum = _ioModule->GetCurStringNum();
    curStrPos = _ioModule->GetCurSymbNum();
    if (word.size() == 1) {
        auto vcToken = new CharValue(word[0]);
        _curToken = new ValueToken(vcToken, curStrNum, curStrPos - 2, curStrPos);
    } else {
        string wordStr(word.begin(), word.end());
        auto vsToken = new StringValue(wordStr);
        _curToken = new ValueToken(vsToken, curStrNum, curStrPos - wordStr.length(), curStrPos);
    }
}

void Lexer::ColonHandler(char symb) {
    int curStrNum, curStrPos;
    vector<char> word;
    char futureChar = _ioModule->CheckNext();

    word.push_back(symb);

    curStrNum = _ioModule->GetCurStringNum();
    curStrPos = _ioModule->GetCurSymbNum();

    if (futureChar == '=') {
        auto sToken = new Operator(":=", _symbMap[":="]);
        _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
        _ioModule->GetNextSymb();
    } else {
        string str = string(1, symb);
        auto it = _symbMap.find(str);
        auto sToken = new Operator(str, _symbMap[str]);
        _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
    }
}

void Lexer::MoreHandler(char symb) {
    char futureChar = _ioModule->CheckNext();
    int curStrNum = _ioModule->GetCurStringNum();
    int curStrPos = _ioModule->GetCurSymbNum();

    if (futureChar == '=') {
        auto sToken = new Operator(">=", _symbMap[">="]);
        _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
        _ioModule->GetNextSymb();
    } else {
        auto sToken = new Operator(">", _symbMap[">"]);
        _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
    }
}

void Lexer::LowerHandler(char symb) {
    char futureChar = _ioModule->CheckNext();
    int curStrNum = _ioModule->GetCurStringNum();
    int curStrPos = _ioModule->GetCurSymbNum();

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
}

void Lexer::LongCommHandler(char symb) {
    int curStrNum, curStrPos;

    while (!_ioModule->IsEnd() && symb != '}') {
        symb = _ioModule->GetNextSymb();
    }
    if (_ioModule->IsEnd()) {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        RaiseError(curStrPos, curStrNum, "Незакрытый комментарий", 4);
    } else {
        _ioModule->GetNextSymb();
    }
    _curToken = GetNextToken();
}

void Lexer::OtherSymbHandler(char symb) {
    int curStrNum = _ioModule->GetCurStringNum();
    int curStrPos = _ioModule->GetCurSymbNum();

    string str = string(1, symb);
    auto it = _symbMap.find(str);
    if (it != _symbMap.end()) {
        auto sToken = new Operator(str, _symbMap[str]);
        _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 1, curStrPos);
    } else {
        RaiseError(curStrPos, curStrNum, "Неопознанный символ: '" + str + "'", 3);
    }
}

Token* Lexer::GetNextToken() {
	char symb = _ioModule->GetCurSymb();
    int curStrNum, curStrPos;

	if (_ioModule->IsEnd()) {
		return NULL;
	}
    
    while(symb == ' ') {
        symb = _ioModule->GetNextSymb();
    }

	if (IsCharacter(symb)) {
        CharacterTokenHandler(symb);
        _ioModule->GetNextSymb();
	} else if (IsDigit(symb)) {
        auto _oldTokenStr = _curToken->GetStrNum();
        auto _oldTokenStart = _curToken->GetStartPos();
        auto _oldTokenType = _curToken->GetType();
        DigitTokenHandler(symb);
        if (_oldTokenStart == _curToken->GetStrNum() && _oldTokenStart == _curToken->GetStartPos() && _oldTokenType == _curToken->GetType()) {
            _curToken = GetNextToken();
        } else {
            _ioModule->GetNextSymb();
        }
	} else if (symb == '\'') {
        auto _oldTokenStr = _curToken->GetStrNum();
        auto _oldTokenStart = _curToken->GetStartPos();
        auto _oldTokenType = _curToken->GetType();
        StringTokenHandler(symb);
        if (_oldTokenStart == _curToken->GetStrNum() && _oldTokenStart == _curToken->GetStartPos() && _oldTokenType == _curToken->GetType()) {
            _curToken = GetNextToken();
        } else {
            _ioModule->GetNextSymb();
        }
    } else if (symb == ':') {
        ColonHandler(symb);
        _ioModule->GetNextSymb();
    } else if (symb == '/') {
        curStrNum = _ioModule->GetCurStringNum();
        curStrPos = _ioModule->GetCurSymbNum();
        auto sToken = new Operator("/", _symbMap["/"]);
        _curToken = new OperatorToken(sToken, curStrNum, curStrPos - 2, curStrPos);
        _ioModule->GetNextSymb();
    } else if (symb == '{') {
        LongCommHandler(symb);
    } else if (symb == '<') {
        LowerHandler(symb);
        _ioModule->GetNextSymb();
    } else if (symb == '>') {
        MoreHandler(symb);
        _ioModule->GetNextSymb();
    } else if (symb == '-') {
        auto _oldTokenStr = _curToken->GetStrNum();
        auto _oldTokenStart = _curToken->GetStartPos();
        auto _oldTokenType = _curToken->GetType();
        DigitTokenHandler(symb);
        if (_oldTokenStart == _curToken->GetStrNum() && _oldTokenStart == _curToken->GetStartPos() && _oldTokenType == _curToken->GetType()) {
            _curToken = GetNextToken();
        }
        _ioModule->GetNextSymb();
    } else {
        auto _oldTokenStr = _curToken->GetStrNum();
        auto _oldTokenStart = _curToken->GetStartPos();
        auto _oldTokenType = _curToken->GetType();
        OtherSymbHandler(symb);
        if (_oldTokenStart == _curToken->GetStrNum() && _oldTokenStart == _curToken->GetStartPos() && _oldTokenType == _curToken->GetType()) {
            _ioModule->GetNextSymb();
            _curToken = GetNextToken();
        } else {
            _ioModule->GetNextSymb();
        }
	}

    return _curToken;
}

Token* Lexer::GetCurToken() {
	return _curToken;
}

void Lexer::RaiseError(int pos, int strNum, string reason, int code) {
    _errors.push_back(Error(strNum + 1, pos + 1, reason, code));
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