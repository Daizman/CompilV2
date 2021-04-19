#include "Syntax.hpp"


Syntax::Syntax(string fIn, string fOut) {
	_ioModule = new IOModule(fIn, fOut);
	_lexer = new Lexer(_ioModule);
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

bool Syntax::CheckIdents(IdentificatorToken* token) {
	for (auto tok : _idents) {
		if (tok->GetValue()->GetName() == token->GetValue()->GetName()) {
			return true;
		}
	}
	return false;
}

void Syntax::SkipConsts(Token* token, IdentificatorToken* identToken, Identificator* ident) {
	_idents.clear();
	while (!_ioModule->IsEnd() && ident->GetName() != "type" && ident->GetName() != "var" && ident->GetName() != "begin") {
		token = _lexer->GetNextToken();
		while (!_ioModule->IsEnd() && token->GetType() != TokenType::IDENTIFICATOR) {
			token = _lexer->GetNextToken();
		}
		if (token->GetType() == TokenType::IDENTIFICATOR) {
			identToken = (IdentificatorToken*)token;
			ident = identToken->GetValue();
		}
	}
}

// <программа>::=program <имя>; <блок>.
void Syntax::BNFProg() {
	IdentificatorToken* identToken;
	OperatorToken* operToken;

	Identificator* ident;
	Operator* oper;

	auto token = _lexer->GetNextToken();
	if (token->GetType() == TokenType::IDENTIFICATOR) {
		identToken = (IdentificatorToken*)token;
		ident = identToken->GetValue();
		if (ident->GetName() == "program") {
			token = _lexer->GetNextToken();
			if (token->GetType() == TokenType::IDENTIFICATOR) {
				identToken = (IdentificatorToken*)token;
				ident = identToken->GetValue();
				token = _lexer->GetNextToken();
				if (token->GetType() == TokenType::OPERATOR) {
					operToken = (OperatorToken*)token;
					oper = operToken->GetValue();
					if (oper->GetSymb() != ";") {
						RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закончено определение программы", 13);
					}
				} else {
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закончено определение программы", 13);
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не задано имя программы", 12);
			}
		} else {
			RaiseError(0, 0, "Не найдено определение программы", 11);
		}
	} else {
		RaiseError(0, 0, "Не найдено определение программы", 11);
	}
	BNFBlock();
}

// <блок>::=<раздел констант><раздел типов><раздел переменных><раздел операторов>
void Syntax::BNFBlock() {
	IdentificatorToken* identToken;
	Identificator* ident;

	auto token = _lexer->GetNextToken();
	if (token->GetType() == TokenType::IDENTIFICATOR) {
		identToken = (IdentificatorToken*)token;
		ident = identToken->GetValue();
		if (ident->GetName() == "const") {
			BNFConsts();
			token = _lexer->GetCurToken();
		}
		if (token->GetType() == TokenType::IDENTIFICATOR) {
			identToken = (IdentificatorToken*)token;
			ident = identToken->GetValue();
			if (ident->GetName() == "type") {
				BNFTypes();
				token = _lexer->GetCurToken();
			} 
			if (token->GetType() == TokenType::IDENTIFICATOR) {
				identToken = (IdentificatorToken*)token;
				ident = identToken->GetValue();
				if (ident->GetName() == "var") {
					BNFVariants();
					token = _lexer->GetCurToken();
				}
				if (token->GetType() == TokenType::IDENTIFICATOR) {
					identToken = (IdentificatorToken*)token;
					ident = identToken->GetValue();
					if (ident->GetName() != "begin") {
						RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определен основной блок", 15);
					}
				} else {
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определен основной блок", 15);
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определен основной блок", 15);
			}
		} else {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определен основной блок", 15);
		}
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определены блоки", 14);
	}
	BNFOpers();
}

// <раздел констант>::=<пусто>|const <определение константы>; {<определение константы>;}
void Syntax::BNFConsts() {
	//если не было const, не попадем
	IdentificatorToken* identToken;
	OperatorToken* operToken;

	Identificator* ident;
	Operator* oper;

	auto token = _lexer->GetNextToken();
	if (token->GetType() == TokenType::IDENTIFICATOR) {
		identToken = (IdentificatorToken*)token;
		ident = identToken->GetValue();
		while (!_ioModule->IsEnd() && ident->GetName() != "type" && ident->GetName() != "var" && ident->GetName() != "begin") {
			BNFConstDif();
			token = _lexer->GetNextToken();
			if (token->GetType() == TokenType::OPERATOR) {
				operToken = (OperatorToken*)token;
				oper = operToken->GetValue();
				if (oper->GetSymb() != ";") {
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закончено объявление константы", 18);
					SkipConsts(token, identToken, ident);
				} else {
					token = _lexer->GetNextToken();
					if (token->GetType() == TokenType::IDENTIFICATOR) {
						identToken = (IdentificatorToken*)token;
						ident = identToken->GetValue();
					} else {
						RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определен основной блок", 15);
						SkipConsts(token, identToken, ident);
					}
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не закончено объявление константы", 18);
				SkipConsts(token, identToken, ident);
			}
		}
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указано имя константы", 19);
	}
}

// <раздел типов>::=<пусто>|type <определение типа>; {<определение типа>;}
void Syntax::BNFTypes() {

}

// <раздел переменных>::=var <описание однотипных переменных>;{<описание однотипных переменных>;} | <пусто>
void Syntax::BNFVariants() {

}

// <раздел операторов>::=<составной оператор>
void Syntax::BNFOpers() {

}

// <определение константы>::=<имя>=<константа>
void Syntax::BNFConstDif() {
	IdentificatorToken* identToken;
	OperatorToken* operToken;
	ValueToken* valToken;

	Identificator* ident;
	Operator* oper;
	Value* val;

	auto token = _lexer->GetCurToken();
	identToken = (IdentificatorToken*)token;
	ident = identToken->GetValue();

	auto eqToken = _lexer->GetNextToken();
	if (eqToken->GetType() == TokenType::OPERATOR) {
		operToken = (OperatorToken*)eqToken;
		oper = operToken->GetValue();
		if (oper->GetSymb() == "=") {
			auto conToken = _lexer->GetNextToken();
			if (conToken->GetType() == TokenType::VALUE) {
				valToken = (ValueToken*)conToken;
				_idents.push_back(identToken);
			} else if (conToken->GetType() == TokenType::IDENTIFICATOR) {
				auto testIdentToken = (IdentificatorToken*)conToken;
				if (CheckIdents(testIdentToken)) {
					_idents.push_back(identToken);
				} else {
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Объявление константы не заданным идетификатором", 16);
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неверное объявление константы", 17);
			}
		} else {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неверное объявление константы", 17);
		}
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неверное объявление константы", 17);
	}
}

// <определение типа>::=<имя>=<тип>
void Syntax::BNFTypeDif() {
	
}

// <тип>::=<простой тип>
void Syntax::BNFType() {
	
}

// <простой тип>::=<перечислимый тип>|<ограниченный тип>|<имя типа>
void Syntax::BNFSimpleType() {

}

// <перечислимый тип>::=(<имя>{,<имя>})
void Syntax::BNFEnumType() {

}

// <ограниченный тип>::=<константа>..<константа>
void Syntax::BNFGroundedType() {
	
}

// <имя типа>::=<имя>
void Syntax::BNFTypeName() {
	
}

// <описание однотипных переменных>::=<имя>{,<имя>}:<тип>
void Syntax::BNFSingleTypeVariants() {
	
}

// <составной оператор>::= begin <оператор>{;<оператор>} end
void Syntax::BNFConcOper() {
	
}

// <оператор>::=<непомеченный оператор>
void Syntax::BNFOper() {

}

// <непомеченный оператор>::=<простой оператор>|<сложный оператор>
void Syntax::BNFNotMarkedOper() {

}

// <простой оператор>::=<оператор присваивания>|<оператор процедуры>|<пустой оператор>
void Syntax::BNFSimpleOper() {

}

// <оператор присваивания>::=<переменная>:=<выражение>|<имя функции>: = <выражение>
void Syntax::BNFOperAssigm() {

}

// <переменная>::=<полная переменная>|<указанная переменная>
void Syntax::BNFVar() {
	
}

// <полная переменная>::=<имя переменной>
void Syntax::BNFFullVar() {
	
}

// <имя переменной>::=<имя>
void Syntax::BNFVarName() {

}

// <указанная переменная>::=<переменная-ссылка>
void Syntax::BNFPointedVar() {
	
}

// <переменная-ссылка>::=<переменная>
void Syntax::BNFPointerVar() {
	
}

// <выражение>::=<простое выражение>|<простое выражение><операция отношения><простое выражение>
void Syntax::BNFExpres() {
	
}

// <операция отношения>::==|<>|<|<=|>=|>|in
void Syntax::BNFComperExpr() {
	
}

// <простое выражение>::=<знак><слагаемое>{<аддитивная операция><слагаемое>}
void Syntax::BNFSimpleExpr() {

}

// <аддитивная операция>::=+|-|or
void Syntax::BNFAdditOper() {

}

// <слагаемое>::=<множитель>{<мультипликативная операция><множитель>}
void Syntax::BNFSlag() {
	
}

// <мультипликативная операция>::=*|/|div|mod|and
void Syntax::BNFMultOper() {

}

// <множитель>::=<переменная>|<константа без знака>|(<выражение>) | <обозначение функции> | <множество> | not <множитель>
void Syntax::BNFMultiplier() {

}

// <обозначение функции>::=<имя функции>|<имя функции>(<фактический параметр>{, <фактический параметр>})
void Syntax::BNFFuncDif() {

}

// <имя функции>::=<имя>
void Syntax::BNFFuncName() {
	
}

// <фактический параметр>::=<выражение>|<переменная>|<имя процедуры> | <имя функции>
void Syntax::BNFFactParam() {

}

// <множество>::=[<список элементов>]
void Syntax::BNFSet() {

}

// <список элементов>:: = <элемент>{ ,<элемент> } | <пусто>
void Syntax::BNFElemsList() {

}

// <элемент>:: = <выражение> | <выражение>..<выражение>
void Syntax::BNFElement() {

}

// <имя процедуры>::=<имя>
void Syntax::BNFProcName() {

}

// <оператор процедуры>::=<имя процедуры>|<имя процедуры>(<фактический параметр>{, <фактический параметр>})
void Syntax::BNFProcOper() {

}

// <сложный оператор>::=<составной оператор>|<выбирающий оператор>
void Syntax::BNFBigOper() {
	
}

// <выбирающий оператор>::=<условный оператор>
void Syntax::BNFChooseOper() {
	
}

// <условный оператор>::= if <выражение> then <оператор>|if <выражение> then <оператор> else <оператор>
void Syntax::BNFIfOper() {

}
