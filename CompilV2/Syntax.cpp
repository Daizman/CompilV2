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
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ошибка", 17);
		return false;
	}
	if (_curToken->GetType() != TokenType::OPERATOR) {
		if (lex == "program") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не найдено определение программы", 10);
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
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Нет ';'", 11);
		}
		if (lex == "=") {

		}
		if (lex == ",") {

		}
		if (lex == ":=") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ошибка при присваивании значения переменной", 64);
		}
		if (lex == ".") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ожидалась . в качестве завершения программы", 15);
		}
		return false;
	}
	if (_curToken->GetType() == TokenType::OPERATOR) {
		if (((OperatorToken*)_curToken)->GetValue()->GetSymb() != lex) {
			if (lex == "program") {
				RaiseError(0, 0, "Не найдено определение программы", 10);
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
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Нет ';'", 11);
			}
			if (lex == "=") {

			}
			if (lex == ",") {

			}
			if (lex == ":=") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ошибка при присваивании значения переменной", 64);
			}
			if (lex == ".") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ожидалась . в качестве завершения программы", 15);
			}
			return false;
		}
	}
	NextToken();
	return true;
}

bool Syntax::CheckLexemWS(string lex) {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ошибка", 17);
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
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ошибка", 17);
		return false;
	}
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		if (ident == "end") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закрыт обязательный блок", 41);
		}
		if (ident == "begin") {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
		}
		return false;
	}
	if (_curToken->GetType() == TokenType::IDENTIFICATOR) {
		if (((IdentificatorToken*)_curToken)->GetValue()->GetName() != ident) {
			if (ident == "end") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закрыт обязательный блок", 41);
			}
			if (ident == "begin") {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
			}
			return false;
		}
	}
	NextToken();
	return true;
}

bool Syntax::CheckIdentWS(string ident) {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Ошибка", 17);
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
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не задано имя программы", 12);
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

// <программа>::=program <имя>; <блок>.
void Syntax::BNFProg() {
	NextToken();
	
	if (!CheckLexem("program") || EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не задано имя программы", 12);
		return;
	}

	if (!GetProgName() || EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Нет ';'", 11);
		return;
	}

	if (!CheckLexem(";") || EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
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
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Текст после окончания программы", 30);
	}
}

// <блок>::=<раздел констант><раздел переменных><раздел операторов>
void Syntax::BNFBlock() {
	IdentificatorToken* identToken;
	Identificator* ident;

	vector<string> constsBlock = { "const", "var", "begin" };
	vector<string> variantsBlock = { "var", "begin" };
	vector<string> opersBlock = { "begin" };

	if (!SkipToOneOfIdents(constsBlock)) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
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
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
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
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
		return;
	}

	BNFOpers();
}

// <раздел констант>::=<пусто>|const <определение константы>; {<определение константы>;}
void Syntax::BNFConsts() {
	vector<string> constsBlock = { "const", "var", "begin" };
	if (EmptyToken() || _curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указаны имена констант", 50);
		SkipToOper(";");
		SkipToOneOfIdents(constsBlock);
	}
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
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

// <раздел переменных>::=var <описание однотипных переменных>;{<описание однотипных переменных>;} | <пусто>
void Syntax::BNFVariants() {
	vector<string> variantsBlock = { "var", "begin" };
	if (EmptyToken() || _curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указаны имена переменных", 60);
		SkipToOper(";");
		SkipToOneOfIdents(variantsBlock);
	}
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
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

// <раздел операторов>::=<составной оператор>
void Syntax::BNFOpers() {
	BNFConcOper();
}

// <определение константы>::=<имя>=<константа>
void Syntax::BNFConstDif() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
		return;
	}

	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неправильно указаны имена констант", 51);
		return;
	}

	if (StrVecChecker({ "const", "var", "begin" }, ((IdentificatorToken*)_curToken)->GetValue()->GetName()) != -1) {
		return;
	}

	NextToken();
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
		return;
	}

	if (_curToken->GetType() != TokenType::OPERATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указано значение константы", 52);
		return;
	}

	if (_curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() != "=") {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указано значение константы", 52);
		return;
	}

	NextToken();

	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указано значение константы", 52);
		return;
	}

	if (_curToken->GetType() != TokenType::VALUE) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указано значение константы", 52);
		return;
	}
	NextToken();
}

// <описание однотипных переменных>::=<имя>{,<имя>}:<тип>
void Syntax::BNFSingleTypeVariants() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
		return;
	}

	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неправильно указаны имена переменных", 61);
		return;
	}

	if (StrVecChecker({ "var", "begin" }, ((IdentificatorToken*)_curToken)->GetValue()->GetName()) != -1) {
		return;
	}

	while (!EmptyToken() && _curToken->GetType() == TokenType::IDENTIFICATOR) {
		NextToken();
		if (EmptyToken()) {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
			return;
		}
		if (_curToken->GetType() != TokenType::OPERATOR) {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неправильно указаны имена переменных", 61);
			return;
		}
		if (((OperatorToken*)_curToken)->GetValue()->GetSymb() == ",") {
			NextToken();
		}
	}
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указан тип переменных", 62);
		return;
	}
	if (_curToken->GetType() != TokenType::OPERATOR || (_curToken->GetType() == TokenType::OPERATOR && ((OperatorToken*)_curToken)->GetValue()->GetSymb() != ":")) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указан тип переменных", 62);
		return;
	}
	NextToken();
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указан тип переменных", 62);
		return;
	}
	auto iTokenVal = ((IdentificatorToken*)_curToken)->GetValue()->GetName();
	if (iTokenVal != "integer" && iTokenVal != "double" && iTokenVal != "char" && iTokenVal != "string") {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неправильно указан тип переменных", 63);
		return;
	}
	NextToken();
}

// <составной оператор>::= begin <оператор>{;<оператор>} end
void Syntax::BNFConcOper() {
	/*if (!CheckIdentWS("begin")) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не определен следующий обязательный блок", 40);
		return;
	}*/
	NextToken();
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закрыт обязательный блок", 41);
		return;
	}
	if (_curToken->GetType() == TokenType::VALUE) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Одиночное значение", 42);
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

// <оператор>::=<непомеченный оператор>
void Syntax::BNFOper() {
	BNFNotMarkedOper();
}

// <непомеченный оператор>::=<простой оператор>|<сложный оператор>
void Syntax::BNFNotMarkedOper() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закрыт обязательный блок", 41);
		return;
	}

	if (_curToken->GetType() == TokenType::VALUE) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Одиночное значение", 42);
		return;
	}

	if (CheckIdentWS("begin")) {
		BNFDiffExpr();
		return;
	}
	BNFSimpleOper();
	CheckLexem(";");
}

// <простой оператор>::=<оператор присваивания>
void Syntax::BNFSimpleOper() {
	if (CheckLexemWS(";") || CheckIdentWS("end")) {
		return;
	}
	BNFOperAssigm();
}

// <сложный оператор>::=<оператор составной>
void Syntax::BNFDiffExpr() {
	BNFConcOper();
}

// <оператор присваивания>::=<переменная>:=<выражение>
void Syntax::BNFOperAssigm() {
	BNFVar();
	CheckLexem(":=");
	BNFExpres();
}

// <переменная>::=<полная переменная>
void Syntax::BNFVar() {
	BNFFullVar();
}

// <полная переменная>::=<имя переменной>
void Syntax::BNFFullVar() {
	if (_curToken->GetType() != TokenType::IDENTIFICATOR) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не задано имя переменной", 63);
		return;
	}
	BNFVarName();
}

// <имя переменной>::=<имя>
void Syntax::BNFVarName() {
	NextToken();
}

// <выражение>::=<простое выражение>
void Syntax::BNFExpres() {
	BNFSimpleExpr();
}

// <простое выражение>::=<знак><слагаемое>{<аддитивная операция><слагаемое>}
void Syntax::BNFSimpleExpr() {
	BNFSlag();
	while (!EmptyToken() && IsAdditive()) {
		NextToken();
		BNFSlag();
	}
}

// <аддитивная операция>::=+|-
void Syntax::BNFAdditOper() {

}

// <слагаемое>::=<множитель>{<мультипликативная операция><множитель>}
void Syntax::BNFSlag() {
	BNFMultiplier();
	while (!EmptyToken() && IsMultipl()) {
		NextToken();
		BNFMultiplier();
	}
}

// <мультипликативная операция>::=*|/
void Syntax::BNFMultOper() {

}

// <множитель>::=<переменная>|<константа без знака>|(<выражение>)
void Syntax::BNFMultiplier() {
	if (EmptyToken()) {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закрыт обязательный блок", 41);
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
