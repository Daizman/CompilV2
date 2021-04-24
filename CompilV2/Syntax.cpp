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
	while (!_ioModule->IsEnd() && ident->GetName() != "var" && ident->GetName() != "begin") {
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

void Syntax::SkipVariants(Token* token, IdentificatorToken* identToken, Identificator* ident) {
	while (!_ioModule->IsEnd() && ident->GetName() != "begin") {
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

void Syntax::CheckEndVariants(vector<IdentificatorToken*> _addedIdents) {
	auto tToken = _lexer->GetNextToken();
	if (tToken->GetType() == TokenType::IDENTIFICATOR) {
		auto tIdentToken = (IdentificatorToken*)tToken;
		auto tIdent = tIdentToken->GetValue();
		if (tIdent->GetName() == "integer" || tIdent->GetName() == "real" || tIdent->GetName() == "string" || tIdent->GetName() == "char") {
			auto endToken = _lexer->GetNextToken();
			if (endToken->GetType() == TokenType::OPERATOR) {
				auto endOperIdentToken = (OperatorToken*)endToken;
				auto endOperIdent = endOperIdentToken->GetValue();
				if (endOperIdent->GetSymb() == ";") {
					auto toBegin = _lexer->GetNextToken();
					if (toBegin->GetType() == TokenType::IDENTIFICATOR) {
						auto toBeginIdentToken = (IdentificatorToken*)toBegin;
						auto toBeginIdent = toBeginIdentToken->GetValue();
						if (toBeginIdent->GetName() != "begin") {
							RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определен основной блок", 15);
							_addedIdents.clear();
						}
					}
				} else {
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не закончен блок объявления переменных", 113);
					_addedIdents.clear();
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не закончен блок объявления переменных", 113);
				_addedIdents.clear();
			}
		} else {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не верный идентификатор типа", 112);
			_addedIdents.clear();
		}
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не верный идентификатор типа", 112);
		_addedIdents.clear();
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

// <блок>::=<раздел констант><раздел переменных><раздел операторов>
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
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "В программе не определены блоки", 14);
	}
	BNFOpers();
}

// <раздел констант>::=<пусто>|const <определение константы>; {<определение константы>;}
void Syntax::BNFConsts() {
	// если не было const, не попадем
	IdentificatorToken* identToken;
	OperatorToken* operToken;

	Identificator* ident;
	Operator* oper;

	auto token = _lexer->GetNextToken();
	if (token->GetType() == TokenType::IDENTIFICATOR) {
		identToken = (IdentificatorToken*)token;
		ident = identToken->GetValue();
		while (!_ioModule->IsEnd() && ident->GetName() != "var" && ident->GetName() != "begin") {
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

// <раздел переменных>::=var <описание однотипных переменных>;{<описание однотипных переменных>;} | <пусто>
void Syntax::BNFVariants() {
	// если не было var, не попадем
	IdentificatorToken* identToken;
	OperatorToken* operToken;

	Identificator* ident;
	Operator* oper;

	auto token = _lexer->GetNextToken();
	if (token->GetType() == TokenType::IDENTIFICATOR) {
		identToken = (IdentificatorToken*)token;
		ident = identToken->GetValue();
		BNFSingleTypeVariants();
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Не указано имя переменной", 110);
	}
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

// <описание однотипных переменных>::=<имя>{,<имя>}:<тип>
void Syntax::BNFSingleTypeVariants() {
	IdentificatorToken* identToken;
	OperatorToken* operToken;
	ValueToken* valToken;

	Identificator* ident;
	Operator* oper;
	Value* val;

	auto token = _lexer->GetCurToken();
	identToken = (IdentificatorToken*)token;
	ident = identToken->GetValue();

	vector<IdentificatorToken*> addedIdents;

	auto dToken = _lexer->GetNextToken();
	if (dToken->GetType() == TokenType::OPERATOR) {
		operToken = (OperatorToken*)dToken;
		oper = operToken->GetValue();
		while (!_ioModule->IsEnd() && oper->GetSymb() != ":") {
			token = _lexer->GetNextToken();
			if (token->GetType() == TokenType::IDENTIFICATOR) {
				identToken = (IdentificatorToken*)token;
				ident = identToken->GetValue();
				addedIdents.push_back(identToken);
				dToken = _lexer->GetNextToken();
				if (dToken->GetType() == TokenType::OPERATOR) {
					operToken = (OperatorToken*)dToken;
					oper = operToken->GetValue();
					if (oper->GetSymb() != ":" || oper->GetSymb() != ",") {
						SkipVariants(token, identToken, ident);
						RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неверное объявление переменной", 111);
						addedIdents.clear();
					}
				}
			} else {
				SkipVariants(token, identToken, ident);
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неверное объявление переменной", 111);
				addedIdents.clear();
			}
		}
		if (oper->GetSymb() != ":") {
			CheckEndVariants(addedIdents);
		}
	} else {
		SkipVariants(token, identToken, ident);
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "Неверное объявление переменной", 111);
	}
	for (auto id : addedIdents) {
		_idents.push_back(id);
	}
}

// <составной оператор>::= begin <оператор>{;<оператор>} end
void Syntax::BNFConcOper() {
	
}

// <оператор>::=<непомеченный оператор>
void Syntax::BNFOper() {

}

// <непомеченный оператор>::=<простой оператор>
void Syntax::BNFNotMarkedOper() {

}

// <простой оператор>::=<оператор присваивания>|<пустой оператор>
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

// <выражение>::=<простое выражение>
void Syntax::BNFExpres() {
	
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

// <множитель>::=<переменная>|<константа без знака>|(<выражение>)
void Syntax::BNFMultiplier() {

}
