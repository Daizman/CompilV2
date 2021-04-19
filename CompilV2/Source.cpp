#include "Lexer.hpp"


int main() {
	setlocale(LC_ALL, "Russian");
	//auto lexer = new Lexer("testInput1.txt", "output");
	//auto lexer = new Lexer("test1.pas", "output");
	auto lexer = new Lexer("errors.pas", "output");
	while (lexer->GetNextToken() != NULL) {
		cout << lexer->GetCurToken()->ToString();
	}
	lexer->PrintErrors();
	return 0;
}