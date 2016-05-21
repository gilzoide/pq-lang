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

#include "Code.hpp"
#include "Symbol.hpp"

namespace pq {

Code::~Code () {
	for (auto sym : code) {
		delete sym;
	}
}


Code& Code::operator<< (const string& sym) {
	code.append (new Symbol (sym));

	return *this;
}


Symbol *Code::getFuncSym () const {
	return code.getFirst ()->as<Symbol> ();
}


Cons *Code::getArguments () const {
	return code.front ()->next;
}

}
