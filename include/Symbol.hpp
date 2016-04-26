#pragma once

#include "Atom.hpp"

#include <string>
#include <unordered_set>

using namespace std;

/**
 * Symbol representation, plain old C strings
 */
using symbol = const char *;
/**
 * Symbol set, the interned symbols
 *
 * @note We store the string versions of symbols because they have a builtin
 * hash function, used for storing in the set
 */
using symbolSet = unordered_set<string>;

/**
 * Symbol wrapper for Func OO
 *
 * This class is also responsible for interning the symbols, which will be used
 * as the Table keys
 */
class Symbol : public Atom {
public:
	/**
	 * Ctor, with Symbol
	 */
	Symbol (const string& sym);

	/**
	 * Dtor
	 */
	~Symbol ();

	/**
	 * Clone function override
	 */
	Atom *clone () override;

	/**
	 * GETTER for the symbol
	 */
	symbol getSym ();

	/**
	 * Function to get the canonical symbol
	 *
	 * @param sym Symbol string
	 */
	static symbol getSymbol (const string& sym);


private:
	/// Our symbol
	symbol sym;

	/// Interned symbols, for canonical representation
	static symbolSet internSymbols;
};
