#pragma once
#include "Scope.h"



class Semantic {
private:
	vector<Scope*> _scopes;
	IOModule* _ioMod;
public:
	Semantic(IOModule*);
	~Semantic();
	void CreateMainScope();
	void CreateScope();
	Scope* GetLastScope();
};

