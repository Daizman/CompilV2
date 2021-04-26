#include "Syntax.hpp"


Syntax::Syntax(string fIn, string fOut) {
	_ioModule = new IOModule(fIn, fOut);
	_lexer = new Lexer(_ioModule);
	_curToken = NULL;
	_progName = "";
}

Syntax::~Syntax() {
	delete _ioModule;
	delete _lexer;
}

Lexer* Syntax::GetLexer() {
	return _lexer;
}

void Syntax::RaiseError(int pos, int strNum, string reason, int code) {
	auto error = Error(strNum + 1, pos + 1, reason, code);
	_errors.push_back(error);
}

void Syntax::PrintErrors() {
	_ioModule->PrintErrors(_errors);
}

void Syntax::NextToken() {
	_curToken = _lexer->GetNextToken();
}

int Syntax::StrVecChecker(vector<string> strs, string str) {
	int i = 0;
	for (auto tmpStr : strs) {
		if (tmpStr == str) {
			return i;
		}
		i++;
	}
	return -1;
}

bool Syntax::IsAdditive() {
	return CheckLexemWS("+") || CheckLexem("-");
}

bool Syntax::IsMultipl() {
	return CheckLexemWS("*") || CheckLexem("/");
}

bool Syntax::CheckLexem(string lex) {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "������", 17);
		return false;
	}
	if (_curToken->GetType() != TokenType::OPERATOR) {
		if (lex == "program") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� ����������� ���������", 10);
		}
		if (lex == ")") {
			
		}
		if (lex == ":") {
		
		}
		if (lex == "of") {
		
		}
		if (lex == "(") {
		
		}
		if (lex == "[") {
		
		}
		if (lex == "]") {
		
		}
		
		if (lex == ";") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��� ';'", 11);
		}
		if (lex == "=") {

		}
		if (lex == ",") {

		}
		if (lex == ":=") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "������ ��� ������������ �������� ����������", 64);
		}
		if (lex == ".") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��������� . � �������� ���������� ���������", 15);
		}
		return false;
	}
	if (_curToken->GetType() == TokenType::OPERATOR) {
		if (((OperatorToken*)_curToken)->GetValue()->GetSymb() != lex) {
			if (lex == "program") {
				RaiseError(0, 0, "�� ������� ����������� ���������", 10);
			}
			if (lex == ")") {

			}
			if (lex == ":") {

			}
			if (lex == "of") {

			}
			if (lex == "(") {

			}
			if (lex == "[") {

			}
			if (lex == "]") {

			}

			if (lex == ";") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��� ';'", 11);
			}
			if (lex == "=") {

			}
			if (lex == ",") {

			}
			if (lex == ":=") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "������ ��� ������������ �������� ����������", 64);
			}
			if (lex == ".") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��������� . � �������� ���������� ���������", 15);
			}
			return false;
		}
	}
	NextToken();
	return true;
}

bool Syntax::CheckLexemWS(string lex) {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "������", 17);
		return false;
	}
	bool checker = false;
	if (_curToken->GetType() == TokenType::OPERATOR) {
		if (((OperatorToken*)_curToken)->GetValue()->GetSymb() != lex) {
			checker = true;
		}
	}
	return !(_curToken->GetType() != TokenType::OPERATOR || checker);
}

bool Syntax::CheckIdent(string ident) {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "������", 17);
		return false;
	}
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		if (ident == "end") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ������������ ����", 41);
		}
		if (ident == "begin") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		}
		return false;
	}
	if (_curToken->GetType() == TokenType::IDENTIFICATOR) {
		if (((IdentificatorToken*)_curToken)->GetValue()->GetName() != ident) {
			if (ident == "end") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ������������ ����", 41);
			}
			if (ident == "begin") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
			}
			return false;
		}
	}
	NextToken();
	return true;
}

bool Syntax::CheckIdentWS(string ident) {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "������", 17);
		return false;
	}
	bool checker = false;
	if (_curToken->GetType() == TokenType::IDENTIFICATOR) {
		if (((IdentificatorToken*)_curToken)->GetValue()->GetName() != ident) {
			checker = true;
		}
	}
	return !(_curToken->GetType() != TokenType::IDENTIFICATOR || checker);
}

bool Syntax::EmptyToken() {
	return _curToken == NULL;
}

bool Syntax::GetProgName() {
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ���������", 12);
		return false;
	}
	_progName = ((IdentificatorToken*)_curToken)->GetValue()->GetName();
	NextToken();
	return true;
}

bool Syntax::SkipToOper(string oper) {
	if (EmptyToken()) {
		return false;
	}
	bool checker = false;
	if (_curToken->GetType() == TokenType::OPERATOR) {
		checker = ((OperatorToken*)_curToken)->GetValue()->GetSymb() != oper;
	}
	while (!EmptyToken() && (_curToken->GetType() != TokenType::OPERATOR || checker)) {
		NextToken();
	}
	if (EmptyToken()) {
		return false;
	}
	return true;
}

bool Syntax::SkipToIdent(string ident) {
	if (EmptyToken()) {
		return false;
	}
	bool checker = false;
	if (_curToken->GetType() == TokenType::IDENTIFICATOR) {
		checker = ((IdentificatorToken*)_curToken)->GetValue()->GetName() != ident;
	}
	while (!EmptyToken() && (_curToken->GetType() != TokenType::IDENTIFICATOR || checker)) {
		NextToken();
	}
	if (EmptyToken()) {
		return false;
	}
	return true;
}

bool Syntax::SkipToOneOfOpers(vector<string> opers) {
	if (EmptyToken()) {
		return false;
	}
	bool checker = false;
	if (_curToken->GetType() == TokenType::OPERATOR) {
		checker = StrVecChecker(opers, ((OperatorToken*)_curToken)->GetValue()->GetSymb()) == -1;
	}

	while (!EmptyToken() && (_curToken->GetType() != TokenType::OPERATOR || checker)) {
		NextToken();
	}
	if (EmptyToken()) {
		return false;
	}
	return true;
}

bool Syntax::SkipToOneOfIdents(vector<string> idents) {
	if (EmptyToken()) {
		return false;
	}
	bool checker = false;
	if (_curToken->GetType() == TokenType::IDENTIFICATOR) {
		checker = StrVecChecker(idents, ((IdentificatorToken*)_curToken)->GetValue()->GetName()) == -1;
	}
	
	while (!EmptyToken() && (_curToken->GetType() != TokenType::IDENTIFICATOR || checker)) {
		NextToken();
	}
	if (EmptyToken()) {
		return false;
	}
	return true;
}

// <���������>::=program <���>; <����>.
void Syntax::BNFProg() {
	NextToken();
	
	if (!CheckLexem("program") || EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ���������", 12);
		return;
	}

	if (!GetProgName() || EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��� ';'", 11);
		return;
	}

	if (!CheckLexem(";") || EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}

	cout << "PROGRAM DIFF END" << endl;
	PrintErrors();
	cout << "------------------" << endl;

	BNFBlock();

	cout << "OPER DIFF END" << endl;
	PrintErrors();
	cout << "------------------" << endl;

	CheckLexem(".");
	if (!EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "����� ����� ��������� ���������", 30);
	}
}

// <����>::=<������ ��������><������ ����������><������ ����������>
void Syntax::BNFBlock() {
	IdentificatorToken* identToken;
	Identificator* ident;

	vector<string> constsBlock = { "const", "var", "begin" };
	vector<string> variantsBlock = { "var", "begin" };
	vector<string> opersBlock = { "begin" };

	if (!SkipToOneOfIdents(constsBlock)) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}
	identToken = (IdentificatorToken*)_curToken;
	ident = identToken->GetValue();

	if (ident->GetName() == "const") {
		NextToken();
		BNFConsts();
	}

	cout << "CONST DIFF END" << endl;
	PrintErrors();
	cout << "------------------" << endl;

	if(!SkipToOneOfIdents(variantsBlock)) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}
	identToken = (IdentificatorToken*)_curToken;
	ident = identToken->GetValue();

	if (ident->GetName() == "var") {
		NextToken();
		BNFVariants();
	}

	cout << "VAR DIFF END" << endl;
	PrintErrors();
	cout << "------------------" << endl;

	if (!SkipToIdent("begin")) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}

	BNFOpers();
}

// <������ ��������>::=<�����>|const <����������� ���������>; {<����������� ���������>;}
void Syntax::BNFConsts() {
	vector<string> constsBlock = { "const", "var", "begin" };
	if (EmptyToken() || _curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� ����� ��������", 50);
		SkipToOper(";");
		SkipToOneOfIdents(constsBlock);
	}
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}
	auto identStr = ((IdentificatorToken*)_curToken)->GetValue()->GetName();
	if (identStr == "const") {
		NextToken();
		BNFConsts();
		return;
	}
	if (identStr == "var" || identStr == "begin") {
		return;
	}
	BNFConstDif();
	while (!EmptyToken() && _curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() == ";") {
		NextToken();
		BNFConstDif();
	}
	if (_curToken->GetType() == TokenType::IDENTIFICATOR && ((IdentificatorToken*)_curToken)->GetValue()->GetName() == "const") {
		NextToken();
		BNFConsts();
	}
}

// <������ ����������>::=var <�������� ���������� ����������>;{<�������� ���������� ����������>;} | <�����>
void Syntax::BNFVariants() {
	vector<string> variantsBlock = { "var", "begin" };
	if (EmptyToken() || _curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� ����� ����������", 60);
		SkipToOper(";");
		SkipToOneOfIdents(variantsBlock);
	}
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}
	auto identStr = ((IdentificatorToken*)_curToken)->GetValue()->GetName();
	if (identStr == "var") {
		BNFVariants();
		return;
	}
	if (identStr == "begin") {
		return;
	}
	BNFSingleTypeVariants();
	while (!EmptyToken() && _curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() == ";") {
		NextToken();
		BNFSingleTypeVariants();
	}
	if (_curToken->GetType() == TokenType::IDENTIFICATOR && ((IdentificatorToken*)_curToken)->GetValue()->GetName() == "var") {
		NextToken();
		BNFVariants();
	}
}

// <������ ����������>::=<��������� ��������>
void Syntax::BNFOpers() {
	BNFConcOper();
}

// <����������� ���������>::=<���>=<���������>
void Syntax::BNFConstDif() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}

	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "����������� ������� ����� ��������", 51);
		return;
	}

	if (StrVecChecker({ "const", "var", "begin" }, ((IdentificatorToken*)_curToken)->GetValue()->GetName()) != -1) {
		return;
	}

	NextToken();
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}

	if (_curToken->GetType() != TokenType::OPERATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}

	if (_curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() != "=") {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}

	NextToken();

	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}

	if (_curToken->GetType() != TokenType::VALUE) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}
	NextToken();
}

// <�������� ���������� ����������>::=<���>{,<���>}:<���>
void Syntax::BNFSingleTypeVariants() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}

	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "����������� ������� ����� ����������", 61);
		return;
	}

	if (StrVecChecker({ "var", "begin" }, ((IdentificatorToken*)_curToken)->GetValue()->GetName()) != -1) {
		return;
	}

	while (!EmptyToken() && _curToken->GetType() == TokenType::IDENTIFICATOR) {
		NextToken();
		if (EmptyToken()) {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
			return;
		}
		if (_curToken->GetType() != TokenType::OPERATOR) {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "����������� ������� ����� ����������", 61);
			return;
		}
		if (((OperatorToken*)_curToken)->GetValue()->GetSymb() == ",") {
			NextToken();
		}
	}
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 62);
		return;
	}
	if (_curToken->GetType() != TokenType::OPERATOR || (_curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() != ":")) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 62);
		return;
	}
	NextToken();
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 62);
		return;
	}
	auto iTokenVal = ((IdentificatorToken*)_curToken)->GetValue()->GetName();
	if (iTokenVal != "integer" && iTokenVal != "double" && iTokenVal != "char" && iTokenVal != "string") {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "����������� ������ ��� ����������", 63);
		return;
	}
	NextToken();
}

// <��������� ��������>::= begin <��������>{;<��������>} end
void Syntax::BNFConcOper() {
	/*if (!CheckIdentWS("begin")) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}*/
	NextToken();
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ������������ ����", 41);
		return;
	}
	if (_curToken->GetType() == TokenType::VALUE) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��������� ��������", 42);
		return;
	}
	while (!EmptyToken() 
		// && _curToken->GetType() != TokenType::OPERATOR 
		&& _curToken->GetType() != TokenType::VALUE
		&& !CheckIdentWS("end")) {
		BNFOper();
	}

	CheckIdent("end");
	if (_curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() == ".") {
		return;
	}
	CheckLexem(";");
}

// <��������>::=<������������ ��������>
void Syntax::BNFOper() {
	BNFNotMarkedOper();
}

// <������������ ��������>::=<������� ��������>|<������� ��������>
void Syntax::BNFNotMarkedOper() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ������������ ����", 41);
		return;
	}

	if (_curToken->GetType() == TokenType::VALUE) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��������� ��������", 42);
		return;
	}

	if (CheckIdentWS("begin")) {
		BNFDiffExpr();
		return;
	}
	BNFSimpleOper();
	CheckLexem(";");
}

// <������� ��������>::=<�������� ������������>
void Syntax::BNFSimpleOper() {
	if (CheckLexemWS(";") || CheckIdentWS("end")) {
		return;
	}
	BNFOperAssigm();
}

// <������� ��������>::=<�������� ���������>
void Syntax::BNFDiffExpr() {
	BNFConcOper();
}

// <�������� ������������>::=<����������>:=<���������>
void Syntax::BNFOperAssigm() {
	BNFVar();
	CheckLexem(":=");
	BNFExpres();
}

// <����������>::=<������ ����������>
void Syntax::BNFVar() {
	BNFFullVar();
}

// <������ ����������>::=<��� ����������>
void Syntax::BNFFullVar() {
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 63);
		return;
	}
	BNFVarName();
}

// <��� ����������>::=<���>
void Syntax::BNFVarName() {
	NextToken();
}

// <���������>::=<������� ���������>
void Syntax::BNFExpres() {
	BNFSimpleExpr();
}

// <������� ���������>::=<����><���������>{<���������� ��������><���������>}
void Syntax::BNFSimpleExpr() {
	BNFSlag();
	while (!EmptyToken() && IsAdditive()) {
		NextToken();
		BNFSlag();
	}
}

// <���������� ��������>::=+|-
void Syntax::BNFAdditOper() {

}

// <���������>::=<���������>{<����������������� ��������><���������>}
void Syntax::BNFSlag() {
	BNFMultiplier();
	while (!EmptyToken() && IsMultipl()) {
		NextToken();
		BNFMultiplier();
	}
}

// <����������������� ��������>::=*|/
void Syntax::BNFMultOper() {

}

// <���������>::=<����������>|<��������� ��� �����>|(<���������>)
void Syntax::BNFMultiplier() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ������������ ����", 41);
		return;
	}

	if (_curToken->GetType() == TokenType::VALUE) {
		NextToken();
		return;
	}

	if (CheckLexemWS("(")) {
		NextToken();
		BNFExpres();
		CheckLexem(")");
		return;
	}

	BNFVar();
}
