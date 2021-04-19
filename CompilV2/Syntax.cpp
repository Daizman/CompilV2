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

// <���������>::=program <���>; <����>.
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
						RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ����������� ���������", 13);
					}
				} else {
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ����������� ���������", 13);
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������ ��� ���������", 12);
			}
		} else {
			RaiseError(0, 0, "�� ������� ����������� ���������", 11);
		}
	} else {
		RaiseError(0, 0, "�� ������� ����������� ���������", 11);
	}
	BNFBlock();
}

// <����>::=<������ ��������><������ �����><������ ����������><������ ����������>
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
						RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "� ��������� �� ��������� �������� ����", 15);
					}
				} else {
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "� ��������� �� ��������� �������� ����", 15);
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "� ��������� �� ��������� �������� ����", 15);
			}
		} else {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "� ��������� �� ��������� �������� ����", 15);
		}
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "� ��������� �� ���������� �����", 14);
	}
	BNFOpers();
}

// <������ ��������>::=<�����>|const <����������� ���������>; {<����������� ���������>;}
void Syntax::BNFConsts() {
	//���� �� ���� const, �� �������
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
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ���������� ���������", 18);
					SkipConsts(token, identToken, ident);
				} else {
					token = _lexer->GetNextToken();
					if (token->GetType() == TokenType::IDENTIFICATOR) {
						identToken = (IdentificatorToken*)token;
						ident = identToken->GetValue();
					} else {
						RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "� ��������� �� ��������� �������� ����", 15);
						SkipConsts(token, identToken, ident);
					}
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ��������� ���������� ���������", 18);
				SkipConsts(token, identToken, ident);
			}
		}
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�� ������� ��� ���������", 19);
	}
}

// <������ �����>::=<�����>|type <����������� ����>; {<����������� ����>;}
void Syntax::BNFTypes() {

}

// <������ ����������>::=var <�������� ���������� ����������>;{<�������� ���������� ����������>;} | <�����>
void Syntax::BNFVariants() {

}

// <������ ����������>::=<��������� ��������>
void Syntax::BNFOpers() {

}

// <����������� ���������>::=<���>=<���������>
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
					RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "���������� ��������� �� �������� ��������������", 16);
				}
			} else {
				RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�������� ���������� ���������", 17);
			}
		} else {
			RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�������� ���������� ���������", 17);
		}
	} else {
		RaiseError(_ioModule->GetCurStringNum(), _ioModule->GetCurSymbNum(), "�������� ���������� ���������", 17);
	}
}

// <����������� ����>::=<���>=<���>
void Syntax::BNFTypeDif() {
	
}

// <���>::=<������� ���>
void Syntax::BNFType() {
	
}

// <������� ���>::=<������������ ���>|<������������ ���>|<��� ����>
void Syntax::BNFSimpleType() {

}

// <������������ ���>::=(<���>{,<���>})
void Syntax::BNFEnumType() {

}

// <������������ ���>::=<���������>..<���������>
void Syntax::BNFGroundedType() {
	
}

// <��� ����>::=<���>
void Syntax::BNFTypeName() {
	
}

// <�������� ���������� ����������>::=<���>{,<���>}:<���>
void Syntax::BNFSingleTypeVariants() {
	
}

// <��������� ��������>::= begin <��������>{;<��������>} end
void Syntax::BNFConcOper() {
	
}

// <��������>::=<������������ ��������>
void Syntax::BNFOper() {

}

// <������������ ��������>::=<������� ��������>|<������� ��������>
void Syntax::BNFNotMarkedOper() {

}

// <������� ��������>::=<�������� ������������>|<�������� ���������>|<������ ��������>
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

// <��������� ����������>::=<����������-������>
void Syntax::BNFPointedVar() {
	
}

// <����������-������>::=<����������>
void Syntax::BNFPointerVar() {
	
}

// <���������>::=<������� ���������>|<������� ���������><�������� ���������><������� ���������>
void Syntax::BNFExpres() {
	
}

// <�������� ���������>::==|<>|<|<=|>=|>|in
void Syntax::BNFComperExpr() {
	
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

// <���������>::=<����������>|<��������� ��� �����>|(<���������>) | <����������� �������> | <���������> | not <���������>
void Syntax::BNFMultiplier() {

}

// <����������� �������>::=<��� �������>|<��� �������>(<����������� ��������>{, <����������� ��������>})
void Syntax::BNFFuncDif() {

}

// <��� �������>::=<���>
void Syntax::BNFFuncName() {
	
}

// <����������� ��������>::=<���������>|<����������>|<��� ���������> | <��� �������>
void Syntax::BNFFactParam() {

}

// <���������>::=[<������ ���������>]
void Syntax::BNFSet() {

}

// <������ ���������>:: = <�������>{ ,<�������> } | <�����>
void Syntax::BNFElemsList() {

}

// <�������>:: = <���������> | <���������>..<���������>
void Syntax::BNFElement() {

}

// <��� ���������>::=<���>
void Syntax::BNFProcName() {

}

// <�������� ���������>::=<��� ���������>|<��� ���������>(<����������� ��������>{, <����������� ��������>})
void Syntax::BNFProcOper() {

}

// <������� ��������>::=<��������� ��������>|<���������� ��������>
void Syntax::BNFBigOper() {
	
}

// <���������� ��������>::=<�������� ��������>
void Syntax::BNFChooseOper() {
	
}

// <�������� ��������>::= if <���������> then <��������>|if <���������> then <��������> else <��������>
void Syntax::BNFIfOper() {

}
