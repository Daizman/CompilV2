#include "Semantic.h"

Semantic::Semantic(IOModule* ioMod) {
	_ioMod = ioMod;
	CreateMainScope();
}

Semantic::~Semantic() {
	delete _ioMod;
}

void Semantic::CreateMainScope() {
	auto mainSc = new Scope();
	mainSc->InitMainScpoe(_ioMod);
	_scopes.push_back(mainSc);
}

void Semantic::CreateScope() {
	auto sc = new Scope();
	auto parSc = _scopes.back();
	sc->SetPar(parSc);
}

Scope* Semantic::GetLastScope() {
	return _scopes.back();
}
