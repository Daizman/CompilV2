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
	auto parSc = _scopes.back();
	auto sc = new Scope(parSc, _ioMod, ScopeType::CONSTS);
	sc->SetPar(parSc);
	_scopes.push_back(sc);
}

Scope* Semantic::GetLastScope() {
	return _scopes.back();
}
