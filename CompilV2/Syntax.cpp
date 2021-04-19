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

// <���������>::=program <���>; <����>.
void Syntax::BNFProg() {
	IdentificatorToken* identToken;
	OperatorToken* operToken;
	ValueToken* valToken;

	Identificator* ident;
	Operator* oper;
	Value* val;

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
	OperatorToken* operToken;
	ValueToken* valToken;

	Identificator* ident;
	Operator* oper;
	Value* val;

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

}

// <���������>::=<����� ��� �����>|<����><����� ��� �����> | <��� ���������> | <����><��� ���������> | <������>
void Syntax::BNFConst() {
	
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
