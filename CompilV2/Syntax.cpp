#include "Syntax.hpp"


Syntax::Syntax(string fIn, string fOut) {
	_ioModule = new IOModule(fIn, fOut);
	_lexer = new Lexer(_ioModule);
	_curToken = NULL;
	_progName = "";
	_semant = new Semantic(_ioModule);
}

Syntax::~Syntax() {
	delete _ioModule;
	delete _lexer;
	delete _semant;
}

Lexer* Syntax::GetLexer() {
	return _lexer;
}

void Syntax::RaiseError(int strNum, int pos, string reason, int code) {
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
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� ���������", 17);
		}
		if (lex == ";") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��� ';'", 11);
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
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� ���������", 17);
			}
			if (lex == ";") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��� ';'", 11);
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

ValueType Syntax::VTByString(string vt) {
	if (vt == "integer") {
		return ValueType::INTEGER;
	}
	if (vt == "real") {
		return ValueType::FLOAT;
	}
	if (vt == "char") {
		return ValueType::CHARACTER;
	}
	if (vt == "string") {
		return ValueType::STRING;
	}
	return ValueType::NONE;
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
	_semant->GetLastScope()->ClearScopeIfErrors();
	cout << "------------------" << endl;
	_semant->GetLastScope()->PrintErrors();
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
	_semant->GetLastScope()->ClearScopeIfErrors();
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
	_semant->GetLastScope()->ClearScopeIfErrors();
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
	if (_semant->GetLastScope()->GetScopeType() != ScopeType::CONSTS) {
		_semant->CreateScope();
		_semant->GetLastScope()->SetScopeType(ScopeType::CONSTS);
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
	if (_semant->GetLastScope()->GetScopeType() != ScopeType::VARS) {
		_semant->CreateScope();
		_semant->GetLastScope()->SetScopeType(ScopeType::VARS);
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
	auto lp = ((IdentificatorToken*)_curToken)->GetValue();
	_semant->GetLastScope()->AddName(lp->GetName());
	NextToken();
	if (EmptyToken()) {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
		return;
	}

	if (_curToken->GetType() != TokenType::OPERATOR) {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}

	if (_curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() != "=") {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}

	NextToken();

	if (EmptyToken()) {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}

	if (_curToken->GetType() != TokenType::VALUE) {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� �������� ���������", 52);
		return;
	}
	auto rp = ((ValueToken*)_curToken)->GetValue();
	_semant->GetLastScope()->AddConst(lp->GetName(), rp);
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
	auto lp = ((IdentificatorToken*)_curToken)->GetValue();
	_semant->GetLastScope()->AddName(lp->GetName());

	while (!EmptyToken() && _curToken->GetType() == TokenType::IDENTIFICATOR) {
		NextToken();
		if (EmptyToken()) {
			_semant->GetLastScope()->Clear();
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ��������� ������������ ����", 40);
			return;
		}
		if (_curToken->GetType() != TokenType::OPERATOR) {
			_semant->GetLastScope()->Clear();
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "����������� ������� ����� ����������", 61);
			return;
		}
		if (((OperatorToken*)_curToken)->GetValue()->GetSymb() == ",") {
			NextToken();
			lp = ((IdentificatorToken*)_curToken)->GetValue();
			_semant->GetLastScope()->AddName(lp->GetName());
		}
	}
	if (EmptyToken()) {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 62);
		return;
	}
	if (_curToken->GetType() != TokenType::OPERATOR || (_curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() != ":")) {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 62);
		return;
	}
	NextToken();
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 62);
		return;
	}
	auto iTokenVal = ((IdentificatorToken*)_curToken)->GetValue()->GetName();
	if (iTokenVal != "integer" && iTokenVal != "double" && iTokenVal != "char" && iTokenVal != "string" && iTokenVal != "real") {
		_semant->GetLastScope()->Clear();
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "����������� ������ ��� ����������", 63);
		return;
	}
	auto rp = ((IdentificatorToken*)_curToken)->GetValue();
	_semant->GetLastScope()->AddType(iTokenVal);
	_semant->GetLastScope()->AddVars();
	NextToken();
}

// <��������� ��������>::= begin <��������>{;<��������>} end
void Syntax::BNFConcOper() {
	NextToken();
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ������������ ����", 41);
		return;
	}
	if (_curToken->GetType() == TokenType::VALUE) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "��������� ��������", 42);
		return;
	}
	_semant->CreateScope();
	_semant->GetLastScope()->SetScopeType(ScopeType::BLOCK);
	while (!EmptyToken() 
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
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ����������", 63);
	} else {
		_semant->GetLastScope()->CheckIdentScope(((IdentificatorToken*)_curToken)->GetValue()->GetName());
		NextToken();
	}
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

// <���������>::=<���������>{<����������������� ��������><���������>}
void Syntax::BNFSlag() {
	BNFMultiplier();
	while (!EmptyToken() && IsMultipl()) {
		NextToken();
		BNFMultiplier();
	}
}

// <���������>::=<����������>|<��������� ��� �����>|(<���������>)
void Syntax::BNFMultiplier() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ������������ ����", 41);
		return;
	}

	if (_curToken->GetType() == TokenType::VALUE) {
		_semant->GetLastScope()->CheckInitVarTypeError(((ValueToken*)_curToken)->GetValue()->GetType());
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
