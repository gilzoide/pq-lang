/* PQ programming language backend
 * Copyright (C) <2016> <gilzoide>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Any bugs should be reported to <gilzoide@gmail.com>
 */

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
