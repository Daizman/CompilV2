#include "Lexer.hpp"


int main() {
	setlocale(LC_ALL, "Russian");
	auto lexer = new Lexer("testInput1.txt", "output");
	while (lexer->GetNextToken() != NULL) {
		cout << lexer->GetCurToken()->ToString();
		if (lexer->GetCurToken()->ToString() == "Identificator: end\n") {
			int stop = 1;
		}
	}
	return 0;
}