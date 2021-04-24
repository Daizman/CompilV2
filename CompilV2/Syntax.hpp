#pragma once
#include "Lexer.hpp"


// ������� ������: 
// ��������� scope: ����������������� ��������: true, false
// ����������� - �� ��� ������� ������������
// ���� - ����������, ������� ����� ��������������!!!
// ��������� ����� �������
// ��������� ����� ���������� (�����������)
// ������ ������ �� ����� ������

// ������� ������:
// ��� ������ ����������� ���, ���� ������� ��� ���������
// ��� ������ ��� ���� �������
// ������ ���������� ���������
// ��� ������ ������� ����� ������ ����, ������� ����� ���� �����
// ��������� �� ����. ����� ��� ����� ��������� (var - begin, ��������)
// ��� ��������


class Syntax {
private:
	Lexer* _lexer;
	IOModule* _ioModule;
	vector<IdentificatorToken*> _idents;
	vector<Error> _errors;
	bool CheckIdents(IdentificatorToken*);
	void SkipConsts(Token*, IdentificatorToken*, Identificator*);
	void CheckEndVariants(vector<IdentificatorToken*>);
	void SkipVariants(Token*, IdentificatorToken*, Identificator*);
public:
	Syntax(string, string);
	~Syntax();
	Lexer* GetLexer();
	void PrintErrors();
	void RaiseError(int, int, string, int);
	void BNFProg();
	void BNFBlock();
	void BNFConsts();
	void BNFVariants();
	void BNFOpers();
	void BNFConstDif();
	void BNFSingleTypeVariants();
	void BNFConcOper();
	void BNFOper();
	void BNFNotMarkedOper();
	void BNFSimpleOper();
	void BNFOperAssigm();
	void BNFVar();
	void BNFFullVar();
	void BNFVarName();
	void BNFExpres();
	void BNFSimpleExpr();
	void BNFAdditOper();
	void BNFSlag();
	void BNFMultOper();
	void BNFMultiplier();
};

