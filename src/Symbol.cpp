#include "Symbol.hpp"

Symbol::Symbol (const string& sym) : sym (getSymbol (sym)) {}


Symbol::~Symbol () {}


Atom *Symbol::clone () {
	return new Symbol (sym);
}


symbol Symbol::getSym () {
	return sym;
}


symbol Symbol::getSymbol (const string& sym) {
	// insert string if needed, and return it's C str
	return internSymbols.insert (sym).first->c_str ();
}


// Declare static attributes
symbolSet Symbol::internSymbols;
