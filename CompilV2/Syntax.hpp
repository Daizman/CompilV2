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
public:
	Syntax(string, string);
	~Syntax();
	Lexer* GetLexer();
	void PrintErrors();
	void RaiseError(int, int, string, int);
	void BNFProg();
	void BNFBlock();
	void BNFConsts();
	void BNFTypes();
	void BNFVariants();
	void BNFOpers();
	void BNFConstDif();
	void BNFConst();
	void BNFTypeDif();
	void BNFType();
	void BNFSimpleType();
	void BNFEnumType();
	void BNFGroundedType();
	void BNFTypeName();
	void BNFSingleTypeVariants();
	void BNFConcOper();
	void BNFOper();
	void BNFNotMarkedOper();
	void BNFSimpleOper();
	void BNFOperAssigm();
	void BNFVar();
	void BNFFullVar();
	void BNFVarName();
	void BNFPointedVar();
	void BNFPointerVar();
	void BNFExpres();
	void BNFComperExpr();
	void BNFSimpleExpr();
	void BNFAdditOper();
	void BNFSlag();
	void BNFMultOper();
	void BNFMultiplier();
	void BNFFuncDif();
	void BNFFuncName();
	void BNFFactParam();
	void BNFEmptyOper();
	void BNFEmpty();
	void BNFSet();
	void BNFElemsList();
	void BNFElement();
	void BNFProcName();
	void BNFProcOper();
	void BNFBigOper();
	void BNFChooseOper();
	void BNFIfOper();
};

