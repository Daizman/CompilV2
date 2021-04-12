#include "Operator.hpp"


Operator::Operator(string symb) {
    _symb = symb;
    if (_symb == "*") {
        _type = OperatorType::STAR;
        return;
    }
    if (_symb == "/") {
        _type = OperatorType::SLASH;
        return;
    }
    if (_symb == "=") {
        _type = OperatorType::EQUAL;
        return;
    }
    if (_symb == ",") {
        _type = OperatorType::COMMA;
        return;
    }
    if (_symb == ";") {
        _type = OperatorType::SEMICOLON;
        return;
    }
    if (_symb == ":") {
        _type = OperatorType::COLON;
        return;
    }
    if (_symb == ".") {
        _type = OperatorType::POINT;
        return;
    }
    if (_symb == "^") {
        _type = OperatorType::ARROW;
        return;
    }
    if (_symb == "(") {
        _type = OperatorType::LEFTPAR;
        return;
    }
    if (_symb == ")") {
        _type = OperatorType::RIGHTPAR;
        return;
    }
    if (_symb == "[") {
        _type = OperatorType::LBRACKET;
        return;
    }
    if (_symb == "]") {
        _type = OperatorType::RBRACKET;
        return;
    }
    if (_symb == "{") {
        _type = OperatorType::FLPAR;
        return;
    }
    if (_symb == "}") {
        _type = OperatorType::FRPAR;
        return;
    }
    if (_symb == "<") {
        _type = OperatorType::LATER;
        return;
    }
    if (_symb == ">") {
        _type = OperatorType::GREATER;
        return;
    }
    if (_symb == "<=") {
        _type = OperatorType::LATEREQUAL;
        return;
    }
    if (_symb == ">=") {
        _type = OperatorType::GREATEREQUAL;
        return;
    }
    if (_symb == "<>") {
        _type = OperatorType::LATERGREATER;
        return;
    }
    if (_symb == "+") {
        _type = OperatorType::PLUS;
        return;
    }
    if (_symb == "-") {
        _type = OperatorType::MINUS;
        return;
    }
    if (_symb == "(*") {
        _type = OperatorType::LCOMMENT;
        return;
    }
    if (_symb == "*)") {
        _type = OperatorType::RCOMMENT;
        return;
    }
    if (_symb == ":=") {
        _type = OperatorType::ASSIGN;
        return;
    }
    if (_symb == "..") {
        _type = OperatorType::TWOPOINTS;
        return;
    }
    if (_symb == "ident") {
        _type = OperatorType::IDENT;
        return;
    }
    if (_symb == "intconst") {
        _type = OperatorType::INTC;
        return;
    }
    if (_symb == "floatconst") {
        _type = OperatorType::FLOATC;
        return;
    }
    if (_symb == "charconst") {
        _type = OperatorType::CHARC;
        return;
    }
    if (_symb == "stringconst") {
        _type = OperatorType::STRINGC;
        return;
    }
    if (_symb == "case") {
        _type = OperatorType::CASESY;
        return;
    }
    if (_symb == "else") {
        _type = OperatorType::ELSESY;
        return;
    }
    if (_symb == "goto") {
        _type = OperatorType::GOTOSY;
        return;
    }
    if (_symb == "type") {
        _type = OperatorType::TYPESY;
        return;
    }
    if (_symb == "with") {
        _type = OperatorType::WITHSY;
        return;
    }
    if (_symb == "file") {
        _type = OperatorType::FILESY;
        return;
    }
    if (_symb == "then") {
        _type = OperatorType::THENSY;
        return;
    }
    if (_symb == "until") {
        _type = OperatorType::UNTILSY;
        return;
    }
    if (_symb == "do") {
        _type = OperatorType::DOSY;
        return;
    }
    if (_symb == "if") {
        _type = OperatorType::IFSY;
        return;
    }
}


Operator::~Operator() {
	
}

string Operator::GetSymb() {
	return _symb;
}

string Operator::ToString() {
    return "Operator: " + _symb + "\n";
}