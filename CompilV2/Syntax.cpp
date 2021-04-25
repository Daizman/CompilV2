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

bool Syntax::CheckLexem(string lex) {
	if (_curToken->GetType() != TokenType::OPERATOR || ((OperatorToken*)_curToken)->GetValue()->GetSymb() != lex) {
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
		if (lex == "end") {
		
		}
		if (lex == ";") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��� ';'", 11);
		}
		if (lex == "=") {

		}
		if (lex == "begin") {
		
		}
		if (lex == ",") {

		}
		if (lex == ":=") {

		}
		if (lex == "then") {

		}
		if (lex == "until") {

		}
		if (lex == "do") {

		}
		if (lex == "to" || lex == "downto") {

		}
		if (lex == "if") {

		}
		if (lex == ".") {

		}
		if (lex == "..") {

		}
		return false;
	}
	NextToken();
	return true;
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
	while (!EmptyToken() && _curToken->GetType() != TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() != oper) {
		NextToken();
	}
	if (EmptyToken()) {
		return false;
	}
	return true;
}

bool Syntax::SkipToIdent(string ident) {
	while (!EmptyToken() && _curToken->GetType() != TokenType::IDENTIFICATOR && ((IdentificatorToken*)_curToken)->GetValue()->GetName() != ident) {
		NextToken();
	}
	if (EmptyToken()) {
		return false;
	}
	return true;
}

bool Syntax::SkipToOneOfOpers(vector<string> opers) {
	while (!EmptyToken() && _curToken->GetType() != TokenType::OPERATOR && StrVecChecker(opers, ((OperatorToken*)_curToken)->GetValue()->GetSymb()) == -1) {
		NextToken();
	}
	if (EmptyToken()) {
		return false;
	}
	return true;
}

bool Syntax::SkipToOneOfIdents(vector<string> idents) {
	while (!EmptyToken() && _curToken->GetType() != TokenType::IDENTIFICATOR && StrVecChecker(idents, ((IdentificatorToken*)_curToken)->GetValue()->GetName()) == -1) {
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

	BNFBlock();

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

	if (!SkipToIdent("begin")) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}

	identToken = (IdentificatorToken*)_curToken;
	ident = identToken->GetValue();

	NextToken();
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
	auto identStr = ((IdentificatorToken*)_curToken)->GetValue()->ToString();
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
	if (_curToken->GetType() == TokenType::IDENTIFICATOR && ((IdentificatorToken*)_curToken)->GetValue()->ToString() == "const") {
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
	auto identStr = ((IdentificatorToken*)_curToken)->GetValue()->ToString();
	if (identStr == "var") {
		BNFConsts();
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
	if (_curToken->GetType() == TokenType::IDENTIFICATOR && ((IdentificatorToken*)_curToken)->GetValue()->ToString() == "var") {
		NextToken();
		BNFVariants();
	}
}

// <������ ����������>::=<��������� ��������>
void Syntax::BNFOpers() {

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

	NextToken();
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
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
	
}

// <��������>::=<������������ ��������>
void Syntax::BNFOper() {

}

// <������������ ��������>::=<������� ��������>
void Syntax::BNFNotMarkedOper() {

}

// <������� ��������>::=<�������� ������������>|<������ ��������>
void Syntax::BNFSimpleOper() {

}

// <�������� ������������>::=<����������>:=<���������>|<��� �������>: = <���������>
void Syntax::BNFOperAssigm() {

}

// <����������>::=<������ ����������>|<��������� ����������>
void Syntax::BNFVar() {
	
}

// <������ ����������>::=<��� ����������>
void Syntax::BNFFullVar() {
	
}

// <��� ����������>::=<���>
void Syntax::BNFVarName() {

}

// <���������>::=<������� ���������>
void Syntax::BNFExpres() {
	
}

// <������� ���������>::=<����><���������>{<���������� ��������><���������>}
void Syntax::BNFSimpleExpr() {

}

// <���������� ��������>::=+|-|or
void Syntax::BNFAdditOper() {

}

// <���������>::=<���������>{<����������������� ��������><���������>}
void Syntax::BNFSlag() {
	
}

// <����������������� ��������>::=*|/|div|mod|and
void Syntax::BNFMultOper() {

}

// <���������>::=<����������>|<��������� ��� �����>|(<���������>)
void Syntax::BNFMultiplier() {

}
