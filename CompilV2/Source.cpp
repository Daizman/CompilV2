#include "Syntax.hpp"


void iTest();
void lexerTest();
void syntTest();
void semTest();


int main() {
	setlocale(LC_ALL, "Russian");
	//iTest();
	lexerTest();
	//syntTest();
	//semTest();
	return 0;
}

void iTest() {
	auto inp = new IOModule("testInput1.txt", "output");
	//auto inp = new IOModule("test1.pas", "output");
	//auto inp = new IOModule("errors.pas", "output");
	char symb = inp->GetCurSymb();

	while (!inp->IsEnd()) {
		cout << symb << endl;
		symb = inp->GetNextSymb();
	}
}

void lexerTest() {
	//auto syntax = new Syntax("testInput1.txt", "output");
	//auto syntax = new Syntax("test1.pas", "output");
	auto syntax = new Syntax("errors.pas", "output");
	auto lexer = syntax->GetLexer();
	while (lexer->GetNextToken() != NULL) {
		cout << lexer->GetCurToken()->ToString();
	}
	lexer->PrintErrors();
}

void syntTest() {
	//auto syntax = new Syntax("synt_test0.pas", "output");
	//auto syntax = new Syntax("synt_test1.pas", "output");
	//auto syntax = new Syntax("constTest.pas", "output");
	auto syntax = new Syntax("varsTest.pas", "output");
	syntax->BNFProg();
	syntax->PrintErrors();
	syntax->GetLexer()->PrintErrors();
}

void semTest() {
	auto syntax = new Syntax("synt_test0.pas", "output");
	//auto syntax = new Syntax("synt_test1.pas", "output");
	//auto syntax = new Syntax("constTest.pas", "output");
	//auto syntax = new Syntax("varsTest.pas", "output");
	//auto syntax = new Syntax("sem_test0.pas", "output");
	//auto syntax = new Syntax("sem_test1.pas", "output");
	syntax->BNFProg();
}