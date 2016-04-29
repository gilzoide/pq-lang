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

#include "Symbol.hpp"

Symbol::Symbol (const string& sym) : sym (getCanonic (sym)) {}


Symbol::~Symbol () {}


Atom *Symbol::clone () {
	return new Symbol (sym);
}


symbol Symbol::getSym () {
	return sym;
}


symbol Symbol::getCanonic (const string& sym) {
	// insert string if needed, and return it's C str
	return internSymbols.insert (sym).first->c_str ();
}


// Declare static attributes
symbolSet Symbol::internSymbols;
