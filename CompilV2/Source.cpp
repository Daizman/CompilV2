#include "Syntax.hpp"


int main() {
	setlocale(LC_ALL, "Russian");
	//auto syntax = new Syntax("testInput1.txt", "output");
	//auto syntax = new Syntax("test1.pas", "output");
	auto syntax = new Syntax("errors.pas", "output");
	auto lexer = syntax->GetLexer();
	while (lexer->GetNextToken() != NULL) {
		cout << lexer->GetCurToken()->ToString();
	}
	lexer->PrintErrors();
	return 0;
}