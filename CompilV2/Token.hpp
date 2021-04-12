#pragma once
#include "Operator.hpp"
#include "Identificator.hpp"
#include "Value.hpp"
#include "TokenType.hpp"


class Token {
private:
    TokenType _type;
    int _tokenStrNum;
    int _tokenStartPos;
    int _tokenEndPos;
public:
    virtual string ToString() = 0;
    TokenType GetType();
    int GetStrNum();
    int GetStartPos();
    int GetEndPos();
    Token(TokenType, int, int, int);
    ~Token();
};

class IdentificatorToken : public Token {
private:
    Identificator* _val;
public:
    Identificator* GetValue();
    string ToString() override;
    IdentificatorToken(Identificator*, int, int, int);
    ~IdentificatorToken();
};

class OperatorToken : public Token {
private:
    Operator* _val;
public:
    Operator* GetValue();
    string ToString() override;
    OperatorToken(Operator*, int, int, int);
    ~OperatorToken();
};

class ValueToken : public Token {
private:
    Value* _val;
public:
    Value* GetValue();
    string ToString() override;
    ValueToken(Value*, int, int, int);
    ~ValueToken();
};