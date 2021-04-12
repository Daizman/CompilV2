#include "Identificator.hpp"


Identificator::Identificator(string name) {
	_name = name;
}

Identificator::~Identificator() {

}

string Identificator::GetName() {
	return _name;
}

string Identificator::ToString() {
	return "Identificator: " + _name + "\n";
}