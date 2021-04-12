#pragma once
#include <string>

using std::string;


class Identificator {
private:
	string _name;
public:
	string GetName();
	Identificator(string);
	~Identificator();
};